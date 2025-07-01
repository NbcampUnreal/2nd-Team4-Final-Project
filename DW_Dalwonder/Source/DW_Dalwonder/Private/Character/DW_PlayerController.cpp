#include "Character/DW_PlayerController.h"

#include "DW_GameInstance.h"
#include "DW_GmBase.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/InventoryMenuWidgetBase.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Character/DW_CharacterBase.h"
#include "UI/Widget/BossHUDWidget.h"
#include "UI/Widget/SettingsManager.h"

const FName ADW_PlayerController::Action_MoveForward = TEXT("Move_Forward");
const FName ADW_PlayerController::Action_MoveBackward = TEXT("Move_Backward");
const FName ADW_PlayerController::Action_MoveLeft = TEXT("Move_Left");
const FName ADW_PlayerController::Action_MoveRight = TEXT("Move_Right");
const FName ADW_PlayerController::Action_LookUp = TEXT("LookUp");
const FName ADW_PlayerController::Action_Turn = TEXT("Turn");
const FName ADW_PlayerController::Action_Jump = TEXT("Jump");
const FName ADW_PlayerController::Action_Attack = TEXT("Attack");
const FName ADW_PlayerController::Action_Interact = TEXT("Interact");
const FName ADW_PlayerController::Action_ESC = TEXT("ESC");
const FName ADW_PlayerController::Action_Guard = TEXT("Guard");
const FName ADW_PlayerController::Action_Dodge = TEXT("Dodge");
const FName ADW_PlayerController::Action_Lockon = TEXT("Lockon");
const FName ADW_PlayerController::Action_Skill = TEXT("Skill");
const FName ADW_PlayerController::Action_Skill1 = TEXT("Skill1");
const FName ADW_PlayerController::Action_Skill2 = TEXT("Skill2");
const FName ADW_PlayerController::Action_Skill3 = TEXT("Skill3");
const FName ADW_PlayerController::Action_Ride = TEXT("Ride");

ADW_PlayerController::ADW_PlayerController()
	: InputMappingContext(nullptr),
	MoveForwardAction(nullptr),
	MoveBackwardAction(nullptr),
	MoveLeftAction(nullptr),
	MoveRightAction(nullptr),
	LookUpAction(nullptr),
	TurnAction(nullptr),
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
	UseSkill3Action(nullptr),
	RideAction(nullptr)
{
}

void ADW_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UDW_GameInstance* GI = Cast<UDW_GameInstance>(GetGameInstance()))
	{
		GI->GetSettingsManager()->ApplyKeyBindingsToInputSystem();
	}

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

	ActionMap.Add(Action_MoveForward, MoveForwardAction);
	ActionMap.Add(Action_MoveBackward, MoveBackwardAction);
	ActionMap.Add(Action_MoveLeft, MoveLeftAction);
	ActionMap.Add(Action_MoveRight, MoveRightAction);
	ActionMap.Add(Action_LookUp, LookUpAction);
	ActionMap.Add(Action_Turn, TurnAction);
	ActionMap.Add(Action_Jump, JumpAction);
	ActionMap.Add(Action_Attack, AttackAction);
	ActionMap.Add(Action_Interact, InteractAction);
	ActionMap.Add(Action_ESC, ESCAction);
	ActionMap.Add(Action_Guard, GuardAction);
	ActionMap.Add(Action_Dodge, DodgeAction);
	ActionMap.Add(Action_Lockon, LockonAction);
	ActionMap.Add(Action_Skill, SkillAction);
	ActionMap.Add(Action_Skill1, UseSkill1Action);
	ActionMap.Add(Action_Skill2, UseSkill2Action);
	ActionMap.Add(Action_Skill3, UseSkill3Action);
	ActionMap.Add(Action_Ride, RideAction);

    GetWorldTimerManager().SetTimer(ObstructionTraceTimerHandle, this, &ADW_PlayerController::UpdateObstructionCheck, 0.1f, true);
}

void ADW_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EI = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// if (ESCAction)
		// {
		// 	EI->BindAction(ESCAction, ETriggerEvent::Started, this, &ADW_PlayerController::ToggleESCMenu);
		// }

		if (auto* SM = Cast<UDW_GameInstance>(GetGameInstance())->GetSettingsManager())
		{
			ApplyCustomKeyBindings(SM->GetCustomKeyMap());
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
	UE_LOG(LogTemp, Warning, TEXT("ToggleESCMenu() 호출됨"));
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

void ADW_PlayerController::ApplyCustomKeyBindings(const TMap<FName, FKey>& KeyMap)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->ClearActionBindings();
		for (const TPair<FName, FKey>& Pair : KeyMap)
		{
			const FName& ActionName = Pair.Key;
			UInputAction* Action = FindActionByName(ActionName);
			if (!Action) continue;

			if (ActionName == TEXT("MoveForward"))
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ADW_PlayerController::HandleMoveForward);
			}
			else if (ActionName == TEXT("MoveBackward"))
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ADW_PlayerController::HandleMoveBackward);
			}
			else if (ActionName == TEXT("MoveLeft"))
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ADW_PlayerController::HandleMoveLeft);
			}
			else if (ActionName == TEXT("MoveRight"))
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ADW_PlayerController::HandleMoveRight);
			}
			else if (ActionName == Action_LookUp)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ADW_PlayerController::HandleLookUp);
			}
			else if (ActionName == Action_Turn)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ADW_PlayerController::HandleTurn);
			}
			else if (ActionName == Action_Jump)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ADW_PlayerController::HandleJump);
			}
			else if (ActionName == Action_Attack)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ADW_PlayerController::HandleAttack);
			}
			else if (ActionName == Action_Interact)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ADW_PlayerController::HandleInteract);
			}
			else if (ActionName == Action_ESC)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ADW_PlayerController::ToggleESCMenu);
			}
			else if (ActionName == Action_Guard)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ADW_PlayerController::HandleGuardStart);
				EnhancedInput->BindAction(Action, ETriggerEvent::Completed, this, &ADW_PlayerController::HandleGuardEnd);
			}
			else if (ActionName == Action_Dodge)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ADW_PlayerController::HandleDodge);
			}
			else if (ActionName == Action_Lockon)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ADW_PlayerController::HandleLockon);
			}
			else if (ActionName == Action_Skill)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ADW_PlayerController::HandleSkill);
			}
			else if (ActionName == Action_Skill1)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ADW_PlayerController::HandleSkill1);
			}
			else if (ActionName == Action_Skill2)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ADW_PlayerController::HandleSkill2);
			}
			else if (ActionName == Action_Skill3)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ADW_PlayerController::HandleSkill3);
			}
			else if (ActionName == Action_Ride)
			{
				EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ADW_PlayerController::HandleRide);
			}
		}
	}
}

void ADW_PlayerController::HandleMoveForward(const FInputActionValue& Value)
{
	if (APawn* MyPawn = GetPawn())
	{
		const FRotator ControlRot = GetControlRotation();
		const FVector Forward = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
		MyPawn->AddMovementInput(Forward, Value.Get<float>());
	}
}

void ADW_PlayerController::HandleMoveBackward(const FInputActionValue& Value)
{
	if (APawn* MyPawn = GetPawn())
	{
		const FRotator ControlRot = GetControlRotation();
		const FVector Forward = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
		MyPawn->AddMovementInput(Forward, -Value.Get<float>());
	}
}

void ADW_PlayerController::HandleMoveLeft(const FInputActionValue& Value)
{
	if (APawn* MyPawn = GetPawn())
	{
		const FRotator ControlRot = GetControlRotation();
		const FVector Right = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);
		MyPawn->AddMovementInput(Right, -Value.Get<float>());
	}
}

void ADW_PlayerController::HandleMoveRight(const FInputActionValue& Value)
{
	if (APawn* MyPawn = GetPawn())
	{
		const FRotator ControlRot = GetControlRotation();
		const FVector Right = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);
		MyPawn->AddMovementInput(Right, Value.Get<float>());
	}
}


void ADW_PlayerController::HandleLookUp(const FInputActionValue& Value)
{

}

void ADW_PlayerController::HandleTurn(const FInputActionValue& Value)
{

}
void ADW_PlayerController::HandleJump()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Jump"));
}

void ADW_PlayerController::HandleAttack()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Attack"));
}

void ADW_PlayerController::HandleInteract()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Interact"));
}

void ADW_PlayerController::HandleGuardStart()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Guard Start"));
}

void ADW_PlayerController::HandleGuardEnd()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Guard End"));
}

void ADW_PlayerController::HandleDodge()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Dodge"));
}

void ADW_PlayerController::HandleLockon()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Lockon"));
}

void ADW_PlayerController::HandleSkill()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Skill"));
}

void ADW_PlayerController::HandleSkill1()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Skill 1"));
}

void ADW_PlayerController::HandleSkill2()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Skill 2"));
}

void ADW_PlayerController::HandleSkill3()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Skill 3"));
}

void ADW_PlayerController::HandleRide()
{
	UE_LOG(LogTemp, Log, TEXT("[입력] Ride"));
}

UInputAction* ADW_PlayerController::FindActionByName(FName ActionName) const
{
	if (const TObjectPtr<UInputAction>* Found = ActionMap.Find(ActionName))
	{
		return Found->Get();
	}
	return nullptr;
}
