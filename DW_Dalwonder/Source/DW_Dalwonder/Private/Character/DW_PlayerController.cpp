#include "Character/DW_PlayerController.h"
#include "DW_GmBase.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/InventoryMenuWidgetBase.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/BossHUDWidget.h"

ADW_PlayerController::ADW_PlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	AttackAction(nullptr),
	InteractAction(nullptr),
	ESCAction(nullptr),
	GuardAction(nullptr),
	DodgeAction(nullptr),
	LockonAction(nullptr),
	SkillAction(nullptr),
	UseSkill1Action(nullptr),
	UseSkill2Action(nullptr),
	UseSkill3Action(nullptr)
{
}

void ADW_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* EILPS = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				EILPS->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

    GetWorldTimerManager().SetTimer(ObstructionTraceTimerHandle, this, &ADW_PlayerController::UpdateObstructionCheck, 0.1f, true);
}

void ADW_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ESCAction)
		{
			EnhancedInputComponent->BindAction(ESCAction, ETriggerEvent::Started, this, &ADW_PlayerController::ToggleESCMenu);
		}
	}
}

void ADW_PlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Pawn이 유효하고, 캐릭터일 경우만 HUD 생성
    if (InPawn && HUDWidgetClass && !HUDWidgetInstance)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
        }
    }
    // 입력 모드 설정 및 마우스 숨김
    SetInputMode(FInputModeGameOnly());
    SetShowMouseCursor(false);
}

void ADW_PlayerController::ToggleESCMenu()
{
    ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
    if (!GameMode || !ESCMenuWidgetClass)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("GameMode or ESCMenuWidgetClass is invalid!"));
#endif
        return;
    }

    // 팝업된 UI가 있다면 (다른 UI 또는 이미 열린 ESC 메뉴)
    if (GameMode->GetPopupWidgetCount() > 0)
    {
        // 마지막 팝업 UI를 닫습니다.
        UUserWidget* ClosedWidget = GameMode->CloseLastPopupUI_AndReturn();

        // 닫힌 위젯이 ESC 메뉴 위젯 인스턴스와 같다면 ESC 메뉴가 닫힌 것으로 간주합니다.
        if (ClosedWidget == ESCMenuWidgetInstance)
        {
            ESCMenuWidgetInstance = nullptr;
            bIsESCMenuOpen = false;

            // --- ESC 메뉴가 닫혔으니 캐릭터 입력 활성화 ---
            if (APawn* MyPawn = GetPawn())
            {
                if (ACharacter* MyCharacter = Cast<ACharacter>(MyPawn))
                {
                    if (MyCharacter->GetCharacterMovement())
                    {
                        MyCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking); // 또는 다른 기본 이동 모드
                    }
                    MyCharacter->EnableInput(this); // 이 PlayerController에 대한 입력 활성화
                }
            }
        }
        return; // 다른 UI가 닫혔거나 ESC 메뉴가 닫혔으므로 여기서 함수 종료
    }

    if (!ESCMenuWidgetInstance)
    {
        ESCMenuWidgetInstance = GameMode->ShowPopupUI(ESCMenuWidgetClass);
        bIsESCMenuOpen = true;

        SetShowMouseCursor(true);

        if (APawn* MyPawn = GetPawn())
        {
            if (ACharacter* MyCharacter = Cast<ACharacter>(MyPawn))
            {
                if (MyCharacter->GetCharacterMovement())
                {
                    MyCharacter->GetCharacterMovement()->StopMovementImmediately(); // 즉시 이동 멈춤
                    MyCharacter->GetCharacterMovement()->DisableMovement(); // 이동 비활성화
                }
                MyCharacter->DisableInput(this); // 이 PlayerController에 대한 입력 비활성화
            }
        }
    }
}

void ADW_PlayerController::ShowBossHUD(const FName& BossName, float MaxHP)
{
	if (CachedBossHUD) return;

	if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
	{
		UUserWidget* RawWidget = GM->ShowPopupUI_M(GM->BossHUDWidgetClass);
		UBossHUDWidget* BossHUD = Cast<UBossHUDWidget>(RawWidget);

		if (BossHUD)
		{
			BossHUD->InitBossHUD(BossName, MaxHP);
			CachedBossHUD = BossHUD;
		}
	}
}

void ADW_PlayerController::HideBossHUD()
{
	if (CachedBossHUD)
	{
		if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
		{
			GM->ClosePopupUI(CachedBossHUD);
		}

		CachedBossHUD = nullptr;
	}
}

void ADW_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetWorldTimerManager().ClearTimer(ObstructionTraceTimerHandle);
}

void ADW_PlayerController::UpdateObstructionCheck()
{
    // 1. 이전에 투명 처리한 액터들의 머티리얼을 원래대로 복원
    for (AActor* Actor : PreviouslyHiddenActors)
    {
        if (!IsValid(Actor)) continue;

        TArray<UMeshComponent*> Meshes;
        Actor->GetComponents<UMeshComponent>(Meshes);

        for (int32 i = 0; i < Meshes.Num(); ++i)
        {
            UMeshComponent* Mesh = Meshes[i];
            if (!IsValid(Mesh)) continue;

            if (OriginalMaterials.Contains(Actor))
            {
                const TArray<UMaterialInterface*>& Mats = OriginalMaterials[Actor];
                if (Mats.IsValidIndex(i))
                {
                    // 이전에 저장된 머티리얼을 복원
                    Mesh->SetMaterial(0, Mats[i]);
                }
            }
        }
    }

    // 복원 완료 후 목록 초기화
    PreviouslyHiddenActors.Empty();
    OriginalMaterials.Empty();

    // 2. 현재 플레이어 폰과 카메라 컴포넌트 가져오기
    APawn* MyPawn = GetPawn();
    if (!IsValid(MyPawn)) return;

    UCameraComponent* Camera = MyPawn->FindComponentByClass<UCameraComponent>();
    if (!IsValid(Camera)) return;

    FVector CameraLocation = Camera->GetComponentLocation();
    FVector PlayerLocation = MyPawn->GetActorLocation();

    // 3. 카메라와 플레이어 사이에 있는 액터들을 감지하기 위한 스피어 트레이스 실행
    TArray<FHitResult> Hits;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(MyPawn); // 자기 자신은 감지 대상에서 제외

    float TraceRadius = 20.0f; // 스피어 반지름 (장애물 감지 범위)
    bool bHit = GetWorld()->SweepMultiByChannel(
        Hits,
        CameraLocation,
        PlayerLocation,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(TraceRadius),
        Params
    );

    if (!bHit) return;

    // 4. 감지된 액터들에 대해 머티리얼을 투명 처리
    for (const FHitResult& Hit : Hits)
    {
        AActor* HitActor = Hit.GetActor();
        if (!IsValid(HitActor) || PreviouslyHiddenActors.Contains(HitActor)) continue;

        TArray<UMeshComponent*> Meshes;
        HitActor->GetComponents<UMeshComponent>(Meshes);

        TArray<UMaterialInterface*> BackupMaterials;

        for (UMeshComponent* Mesh : Meshes)
        {
            if (!IsValid(Mesh)) continue;

            // 기존 머티리얼 백업
            BackupMaterials.Add(Mesh->GetMaterial(0));

            // 지정된 Obstruction 머티리얼 인스턴스로 교체
            if (ObstructionMaterialInstance)
            {
                UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(ObstructionMaterialInstance, this);
                Mesh->SetMaterial(0, DynMat);
            }
        }

        if (BackupMaterials.Num() > 0)
        {
            OriginalMaterials.Add(HitActor, BackupMaterials); // 머티리얼 백업 저장
        }

        PreviouslyHiddenActors.Add(HitActor); // 현재 프레임에 숨긴 액터 목록에 추가
    }

#if WITH_EDITOR
    // 5. 디버그용 라인 및 스피어 시각화 (편집기에서만)
    /*DrawDebugLine(GetWorld(), CameraLocation, PlayerLocation, FColor::Red, false, 0.1f, 0, 1.f);
    DrawDebugSphere(GetWorld(), CameraLocation, TraceRadius, 12, FColor::Green, false, 0.1f);*/
#endif
}
