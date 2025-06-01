#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacterCombatState.h"
#include "GameFramework/Character.h"
#include "DW_SkillComponent.h"
#include "DW_AttributeComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Inventory/InventoryComponent.h"
#include "DW_CharacterBase.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UCharacterStatComponent;
class UUserWidget;
class UNiagaraFunctionLibrary;
class UNiagaraSystem;
class UPhysicalMaterial;

UCLASS()
class DW_DALWONDER_API ADW_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADW_CharacterBase();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDW_SkillComponent* SkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDW_AttributeComponent* AttributeComponent;

protected:
	// ▶ 게임 시작 시 초기 설정 (예: 상태 초기화)
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PostInitializeComponents() override;

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

	void PlayMontage(UAnimMontage* Montage, int32 SectionIndex = -1);
	
	AActor* GetWeapon() const { return Weapon->GetChildActor(); }

	UCharacterStatComponent* GetCharacterStatComponent() const { return StatComponent; }
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimInstance* AnimInstance;

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
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

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
	void BlockCharacterControl(bool bShouldBlock, float Length = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Dead();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetIdleState();

	// 공격한 대상 저장하기 위한 Set
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TSet<AActor*> AttackingActors;

	// 현재 전투 상태 (Idle, Attacking 등)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	ECharacterCombatState CurrentCombatState = ECharacterCombatState::Idle;

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

	// 전투 상태 타이머
	UPROPERTY()
	FTimerHandle IdleStateTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bCanCombo = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 CurrentComboIndex = 0;

	// 가드 중 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsGuarding = false;

	// 무적 상태 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsInvincible = false;

	// 전투 중 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsOnCombat = false;

protected:
	UPROPERTY()
	FTimerHandle BlockTimer;

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

#pragma region Interact
public:
	
	FTimerHandle ItemScanTimerHandle;
	
	UFUNCTION()
	void Interact();
	void AddNearbyItem(class AWorldItemActor* Item);
	void RemoveNearbyItem(AWorldItemActor* Item);
	void UpdateClosestItem();
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	UPROPERTY(EditDefaultsOnly, Category = "FootStep")
	TMap<TEnumAsByte<EPhysicalSurface>, UNiagaraSystem*> FootstepVFXMap;

	// 현재 감지된 SurfaceType (0.01초마다 업데이트됨)
	EPhysicalSurface CurrentSurfaceType = SurfaceType_Default;

	// 바닥 정보를 주기적으로 검사하는 타이머
	FTimerHandle FootstepTraceTimerHandle;

	// 현재 바닥의 SurfaceType을 판별하는 함수
	void UpdateFootstepSurface();

	UFUNCTION(BlueprintCallable)
	void SpawnFootstepEffect(const FName FootSocketName) const;


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
