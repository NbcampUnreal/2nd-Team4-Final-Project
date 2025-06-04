#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DW_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;
class UInventoryMenuWidgetBase;
class ADW_CharacterBase;


UCLASS()
class DW_DALWONDER_API ADW_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADW_PlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	//ESC메뉴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ESCMenuWidgetClass;

	UPROPERTY()
	UUserWidget* ESCMenuWidgetInstance;

	bool bIsESCMenuOpen = false;

	// ESC 메뉴 이벤트
	UFUNCTION()
	void ToggleESCMenu();

	void SetInputModeGameOnly();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ESCAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* GuardAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LockonAction;

	// -----------------------------
	// UI 관련
	// -----------------------------
#pragma region UI
public:
	//HUD
	UPROPERTY()
	UUserWidget* HUDWidgetInstance;

protected:
	// HUD
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> HUDWidgetClass;

public:
	// HUD 표시
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
#pragma endregion
};
