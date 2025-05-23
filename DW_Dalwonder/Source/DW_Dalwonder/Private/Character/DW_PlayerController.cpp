#include "Character/DW_PlayerController.h"
#include "DW_GmBase.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/InventoryMenuWidgetBase.h"
#include "EnhancedInputComponent.h"
#include "Character/DW_CharacterBase.h"

ADW_PlayerController::ADW_PlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	AttackAction(nullptr),
	InteractAction(nullptr),
	ESCAction(nullptr),
	SprintAction(nullptr),
	GuardAction(nullptr)
{
	bIsInventoryOpen = false;
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
  
	if (InventoryWidgetClass)
	{
		InventoryWidgetInstance = CreateWidget<UInventoryMenuWidgetBase>(this, InventoryWidgetClass);
		if (InventoryWidgetInstance)
		{
			InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
			InventoryWidgetInstance->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("Inventory Maked."));
		}
	}

    SetInputMode(FInputModeGameOnly());
    SetShowMouseCursor(false);
}

void ADW_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (InventoryInputAction)
		{
			EnhancedInputComponent->BindAction(InventoryInputAction, ETriggerEvent::Started, this, &ADW_PlayerController::ToggleInventoryUI);
            UE_LOG(LogTemp, Warning, TEXT("인벤토리토글 바인딩완."));
		}
	}
}

ADW_CharacterBase* ADW_PlayerController::GetControlledCharacter() const
{
    // 현재 빙의된 Pawn을 가져와서 MyCharacter로 캐스팅
    return Cast<ADW_CharacterBase>(GetPawn());
}

void ADW_PlayerController::RequestInventoryUIUpdate()
{
    ADW_CharacterBase* CurrentCharacter = GetControlledCharacter();
    if (InventoryWidgetInstance && CurrentCharacter)
    {
        UInventoryComponent* CharacterInventory = CurrentCharacter->GetInventoryComponent();
        if (CharacterInventory)
        {
            InventoryWidgetInstance->UpdateInventoryUI(CharacterInventory);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("C++: RequestInventoryUIUpdate - Character has no InventoryComponent!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("C++: RequestInventoryUIUpdate - InventoryWidgetInstance or Character is null!"));
    }
}

void ADW_PlayerController::ToggleInventoryUI()
{
    if (!InventoryWidgetInstance) return;

    if (bIsInventoryOpen)
    {
        InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        SetInputMode(FInputModeGameOnly()); 
        SetShowMouseCursor(false);
        bIsInventoryOpen = false;
        UE_LOG(LogTemp, Warning, TEXT("Inventory Closed."));
    }
    else 
    {
        
        RequestInventoryUIUpdate(); 
        InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        SetInputMode(FInputModeGameAndUI()); 
        SetShowMouseCursor(true);
        bIsInventoryOpen = true;
        UE_LOG(LogTemp, Warning, TEXT("Inventory Opened."));
    }
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

