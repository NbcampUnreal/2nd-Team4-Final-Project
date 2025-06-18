#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DW_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;
class UInventoryMenuWidgetBase;
class ADW_CharacterBase;
class UBossHUDWidget;

UCLASS()
class DW_DALWONDER_API ADW_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADW_PlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	//ESC메뉴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ESCMenuWidgetClass;

	UPROPERTY()
	UUserWidget* ESCMenuWidgetInstance = nullptr;

	bool bIsESCMenuOpen = false;

	// ESC 메뉴 이벤트
	UFUNCTION()
	void ToggleESCMenu();
	void ShowBossHUD(const FString& BossName, float MaxHP);
	void HideBossHUD();

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
	UInputAction* GuardAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LockonAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* UseSkill1Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* UseSkill2Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* UseSkill3Action;

	// -----------------------------
	// UI 관련
	// -----------------------------
#pragma region UI
public:
	//HUD
	UPROPERTY()
	UUserWidget* HUDWidgetInstance = nullptr;

	UPROPERTY()
	UBossHUDWidget* CachedBossHUD;
protected:
	// HUD
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> HUDWidgetClass;
	

#pragma endregion

#pragma region CustomDepth

public:
	void UpdateObstructionCheck();

	FTimerHandle ObstructionTraceTimerHandle;

	TMap<AActor*, TArray<UMaterialInterface*>> OriginalMaterials;

	UPROPERTY()
	TArray<AActor*> PreviouslyHiddenActors;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Obstruction")
	UMaterialInterface* ObstructionMaterialInstance;

#pragma endregion
};
