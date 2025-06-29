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
	void ShowBossHUD(const FName& BossName, float MaxHP);
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* RideAction;

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
	
	// 바인딩에 사용할 액션 이름 상수 정의
	static const FName Action_Move;
	static const FName Action_Look;
	static const FName Action_Jump;
	static const FName Action_Attack;
	static const FName Action_Interact;
	static const FName Action_ESC;
	static const FName Action_Guard;
	static const FName Action_Dodge;
	static const FName Action_Lockon;
	static const FName Action_Skill;
	static const FName Action_Skill1;
	static const FName Action_Skill2;
	static const FName Action_Skill3;
	static const FName Action_Ride;
	
	UFUNCTION()
	void HandleMove(const FInputActionValue& Value);
	UFUNCTION()
	void HandleLook(const FInputActionValue& Value);
	void HandleJump();
	void HandleAttack();
	void HandleInteract();
	void HandleGuardStart();
	void HandleGuardEnd();
	void HandleDodge();
	void HandleLockon();
	void HandleSkill();
	void HandleSkill1();
	void HandleSkill2();
	void HandleSkill3();
	void HandleRide();

	/** 키 바인딩 재적용 함수 */
	void ApplyCustomKeyBindings(const TMap<FName, FKey>& KeyMap);

	/** 액션 이름 → InputAction 참조 매핑 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TMap<FName, TObjectPtr<UInputAction>> ActionMap;

protected:
	// HUD
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> HUDWidgetClass;

	/** 내부용: 이름으로 액션 찾기 */
	UFUNCTION()
	UInputAction* FindActionByName(FName ActionName) const;

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
