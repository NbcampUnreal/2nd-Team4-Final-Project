#include "Character/DW_PlayerController.h"
#include "DW_GmBase.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/InventoryMenuWidgetBase.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/DW_CharacterBase.h"

ADW_PlayerController::ADW_PlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	AttackAction(nullptr),
	InteractAction(nullptr),
	SprintAction(nullptr),
	GuardAction(nullptr),
	DodgeAction(nullptr),
	LockonAction(nullptr),
	ESCAction(nullptr)
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

	//HUD 보여주기
	ShowGameHUD();
    SetInputMode(FInputModeGameOnly());
    SetShowMouseCursor(false);
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

void ADW_PlayerController::ToggleESCMenu()
{
    ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
    if (!GameMode || !ESCMenuWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameMode or ESCMenuWidgetClass is invalid!"));
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

            // Input Mode를 게임 전용으로 복원합니다.
            //SetInputModeGameOnly(); // 별도 함수로 빼서 깔끔하게 처리
        }
        return; // 다른 UI가 닫혔거나 ESC 메뉴가 닫혔으므로 여기서 함수 종료
    }

    // --- 팝업된 UI가 없을 때만 ESC 메뉴를 엽니다 ---
    // ESC 메뉴 위젯 인스턴스가 없다면 새로 생성합니다.
    //if (!ESCMenuWidgetInstance)
    //{
    //    ESCMenuWidgetInstance = CreateWidget<UUserWidget>(this, ESCMenuWidgetClass); // 'this'를 Owner로 지정
    //}

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

void ADW_PlayerController::SetInputModeGameOnly()
{
    FInputModeGameOnly InputModeGameOnly;
    SetInputMode(InputModeGameOnly);
    bShowMouseCursor = false;
}

void ADW_PlayerController::ShowGameHUD()
{
	if (HUDWidgetClass && !HUDWidgetInstance)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
}

