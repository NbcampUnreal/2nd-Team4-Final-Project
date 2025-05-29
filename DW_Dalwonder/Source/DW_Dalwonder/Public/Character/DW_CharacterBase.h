#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacterCombatState.h"
#include "GameFramework/Character.h"
#include "Inventory/InventoryComponent.h"
#include "DW_CharacterBase.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UCharacterStatComponent;
class UUserWidget;

// ✅ 캐릭터의 기본 클래스: 이동, 전투, 입력 처리 등 공통 기능 포함
UCLASS()
class DW_DALWONDER_API ADW_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADW_CharacterBase();

	virtual void Tick(float DeltaTime) override;

protected:
	// ▶ 게임 시작 시 초기 설정 (예: 상태 초기화)
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// ▶ 플레이어 입력 바인딩 (InputAction → 함수 연결)
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// -----------------------------
	// 📌 입력 처리 관련 함수
	// -----------------------------
	UFUNCTION()
	void Move(const FInputActionValue& Value);             // 이동 입력

	UFUNCTION()
	void Look(const FInputActionValue& Value);             // 마우스/패드 시점 회전 입력

	UFUNCTION()
	void StartJump(const FInputActionValue& Value);        // 점프 시작

	UFUNCTION()
	void StopJump(const FInputActionValue& Value);         // 점프 중지

	UFUNCTION()
	void Attack(const FInputActionValue& Value);

	UFUNCTION()
	void Sprint(const FInputActionValue& Value);

	UFUNCTION()
	void Dodge(const FInputActionValue& Value);

	UFUNCTION()
	void Lockon(const FInputActionValue& Value);

	void PlayMontage(UAnimMontage* Montage, int32 SectionIndex = 0) const;
	
	AActor* GetWeapon() const { return Weapon->GetChildActor(); }

	UCharacterStatComponent* GetCharacterStatComponent() const { return StatComponent; }
	
	// -----------------------------
	// 📌 데미지 처리 함수
	// -----------------------------
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	// -----------------------------
	// 📌 카메라 및 무기 관련 컴포넌트
	// -----------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;                        // 카메라 붐(캐릭터 뒤에 부착)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;                              // 실제 시점 카메라

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* Weapon;                          // 캐릭터의 무기 액터

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	UCharacterStatComponent* StatComponent;				   // 캐릭터의 스탯 컴포넌트

	bool bIsSprinting = false;

	bool bCanControl = true;                               // 캐릭터 조작 가능 여부

	// -----------------------------
	// ⚔️ 전투 관련 시스템 (Combat)
	// -----------------------------
#pragma region Combat

public:
	// 전투 상태 변경 (예: Idle → Attacking)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCombatState(ECharacterCombatState NewState);

	// 공격 애니메이션 재생
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void CancelAttack();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndAttack(UAnimMontage* Montage, bool bInterrupted);

	// 패링 상태 설정
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetParrying(bool bNewParrying);

	// 가드 상태 설정
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetGuarding(bool bNewParrying);

	// 무적 상태 설정
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetInvincible(bool bNewParrying);

	// 가드 시작
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartGuard();

	// 가드 종료
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndGuard();

	// 캐릭터 넉백 처리 (피격 반응)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void KnockBackCharacter();

	// 조작 차단 여부 설정 (피격 중 무력화 등)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void BlockCharacterControl(bool bShouldBlock);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Dead();

	UFUNCTION(blueprintCallable, Category = "Combat")
	void SetAttackTimer(UAnimMontage* Montage, int32 SectionIndex = -1);

	// 공격한 대상 저장하기 위한 Set
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TSet<AActor*> AttackingActors;

	// 현재 전투 상태 (Idle, Attacking 등)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	ECharacterCombatState CurrentCombatState = ECharacterCombatState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 ComboIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bCanAttack = true;

	// 기본 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	// 공중 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* FallingAttackMontage;

	// 가드 중 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* GuardAttackMontage;

	// 달리기 중 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* SprintAttackMontage;

	// 피격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* HitMontage;
	
	// 넉백(쓰러지는) 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* KnockBackMontage;

	// 가드 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* GuardMontage;

	// 구르기(회피) 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* DodgeMontage;
	
	// 패링 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* ParryMontage;

	// 사망 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* DeadMontage;

protected:
	// 패링 중 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsParrying = false;

	// 가드 중 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsGuarding = false;

	// 무적 상태 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsInvincible = false;

	UPROPERTY()
	FTimerHandle AttackTimer;

public:
	// -----------------------------
	// 🎯 락온 관련 변수 및 함수
	// -----------------------------

	// 락온 대상
	UPROPERTY(BlueprintReadOnly, Category = "LockOn")
	AActor* LockOnTarget = nullptr;

	// 락온 여부
	UPROPERTY(BlueprintReadOnly, Category = "LockOn")
	bool bIsLockOn = false;

	// 전환 함수
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void SwitchLockOnTarget();

	// 락온 토글 함수
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void ToggleLockOn();
	
	// 락온 회전용 타이머
	FTimerHandle LockOnRotationTimer;

	// 락온 갱신 타이머
	FTimerHandle LockOnMarkerUpdateTimer;

	// 락온 마커 UI 갱신
	UFUNCTION()
	void UpdateLockOnMarkerPosition();
	
	// 회전 처리 함수
	UFUNCTION()
	void UpdateLockOnRotation();

	// 후보 갱신 함수
	UFUNCTION()
	void UpdateLockOnCandidates();


	AActor* FindBestLockOnTarget();
	
	AActor* FindClosestTarget(float MaxDistance = 1500.f);
	
	UPROPERTY()
	TArray<AActor*> LockOnCandidates;

	int32 LockOnIndex = 0;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|UI")
	TSubclassOf<UUserWidget> LockOnWidgetClass;

	UPROPERTY()
	UUserWidget* LockOnWidgetInstance;

#pragma endregion

	// -----------------------------
	// 🙋 상호작용 관련 시스템 (Interact)
	// -----------------------------
#pragma region Interact
	// -----------------------------
	//  상호작용 관련
	// -----------------------------
public:
	
	FTimerHandle ItemScanTimerHandle;
	
	UFUNCTION()
	void Interact();
	void AddNearbyItem(class AWorldItemActor* Item);
	void RemoveNearbyItem(AWorldItemActor* Item);
	void UpdateClosestItem();
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	AWorldItemActor* CurrentItem = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TArray<AWorldItemActor*> NearbyItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float SphereRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY()
	UUserWidget* InteractionWidget;

private:
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractDistance = 300.f;

	AActor* CurrentInteractTarget = nullptr;

#pragma endregion

	// -----------------------------
	// UI 관련 시스템
	// -----------------------------

#pragma region UI
public:
	//타이머
	FTimerHandle HUDUpdateTimerHandle;

	//ESC메뉴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ESCMenuWidgetClass;

	UPROPERTY()
	UUserWidget* ESCMenuWidgetInstance;

	bool bIsESCMenuOpen = false;

public:

	//HUD업데이트함수
	UFUNCTION()
	void UpdateHUD();

	// ESC 메뉴 이벤트
	UFUNCTION()
	void ToggleESCMenu();

#pragma endregion
};
