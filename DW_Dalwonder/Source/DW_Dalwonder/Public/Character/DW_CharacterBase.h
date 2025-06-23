#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacterCombatState.h"
#include "GameFramework/Character.h"
#include "DW_SkillComponent.h"
#include "DW_AttributeComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Inventory/InventoryComponent.h"
#include "UI/Component/Manager/QuestManagerComponent.h"
#include "DW_CharacterBase.generated.h"

class UDW_AnimInstanceBase;
class UCharacterArmorComponent;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UCharacterStatComponent;
class UUserWidget;
class UNiagaraFunctionLibrary;
class UNiagaraSystem;
class UPhysicalMaterial;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

USTRUCT(BlueprintType)
struct FAnimMontageArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> Montages;

	FORCEINLINE UAnimMontage*& operator[](int32 Index)
	{
		return Montages[Index];
	}

	FORCEINLINE const UAnimMontage* operator[](int32 Index) const
	{
		return Montages[Index];
	}
	
	FORCEINLINE int32 Num() const
	{
		return Montages.Num();
	}
};

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

	UFUNCTION(blueprintCallable)
	void Sprint(bool bOnSprint);

	UFUNCTION()
	void Dodge(const FInputActionValue& Value);

	UFUNCTION()
	void Lockon(const FInputActionValue& Value);

	UFUNCTION()
	void Ride(const FInputActionValue& Value);

	void PlayMontage(UAnimMontage* Montage, int32 SectionIndex = -1);
	
	AActor* GetWeapon() const { return Weapon->GetChildActor(); }

	void SetWeapon(AActor* NewWeapon) { Weapon->SetChildActorClass(NewWeapon->GetClass()); }

	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponType(int32 NewWeaponType);

	UFUNCTION(BlueprintCallable)
	void UpdateSkeletalMesh();

	UCharacterStatComponent* GetCharacterStatComponent() const { return StatComponent; }
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// -----------------------------
	// 📌 카메라 및 무기 관련 컴포넌트
	// -----------------------------
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton")
	USkeletalMeshComponent* Helmet;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton")
	USkeletalMeshComponent* Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton")
	USkeletalMeshComponent* Pants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton")
	USkeletalMeshComponent* Glove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton")
	USkeletalMeshComponent* Boots;

protected:
	// 카메라 붐(캐릭터 뒤에 부착)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	// 실제 시점 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	// 캐릭터의 무기 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 WeaponType = 0;

	// 캐릭터의 장비 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Armor")
	UCharacterArmorComponent* ArmorComponent;

	// 캐릭터의 스탯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	UCharacterStatComponent* StatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TArray<TSubclassOf<UAnimInstance>> AnimInstanceArray;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	// 캐릭터 조작 가능 여부
	bool bCanControl = true;
	
// 전투 관련 시스템 (Combat)
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

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UseActiveSkill();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UseActiveSkillSlot1();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UseActiveSkillSlot2();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UseActiveSkillSlot3();

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
	TArray<UAnimMontage*> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<UAnimMontage*> CancelAttackMontage;

	// 공중 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<UAnimMontage*> FallingAttackMontage;

	// 가드 중 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<UAnimMontage*> GuardAttackMontage;

	// 달리기 중 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<UAnimMontage*> SprintAttackMontage;

	// 피격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<UAnimMontage*> HitMontage;
	
	// 넉백(쓰러지는) 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<UAnimMontage*> KnockBackMontage;

	// 가드 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<UAnimMontage*> GuardMontage;
	
	// 패링 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<UAnimMontage*> ParryMontage;

	// 사망 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<UAnimMontage*> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<FAnimMontageArray> SkillMontage;

	// 전투 상태 타이머
	UPROPERTY()
	FTimerHandle IdleStateTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DodgeDistance = 600.f;

	UPROPERTY()
	FTimerHandle DodgeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float InvincibleDuration = 1.5f;

	UPROPERTY()
	FTimerHandle InvincibleTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bCanCombo = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 CurrentComboIndex = 0;
	
	// 달리기 중 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsSprinting = false;

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
	
	AActor* FindClosestTarget(float MaxDistance = 800.f);
	
	UPROPERTY()
	TArray<AActor*> LockOnCandidates;

	int32 LockOnIndex = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|UI")
	TSubclassOf<UUserWidget> LockOnWidgetClass;

	UPROPERTY()
	UUserWidget* LockOnWidgetInstance;

#pragma endregion

// 탈것 관련 시스템
#pragma region Riding
public:
	UFUNCTION()
	void RideVehicle(bool bOnRiding);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	USkeletalMeshComponent* Vehicle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	USkeletalMeshComponent* Reins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	USkeletalMeshComponent* Saddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	USkeletalMeshComponent* SaddleBelts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	USkeletalMeshComponent* Hair;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	UAnimMontage* RidingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	UAnimMontage* GetOffMontage;
	
	bool bCanRideVehicle = true;

	bool bIsRidingVehicle = false;
#pragma endregion
	
// Interaction 관련 시스템
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
	
// UI 관련 시스템
#pragma region UI
public:
	//타이머
	FTimerHandle HUDUpdateTimerHandle;

	// 퀘스트 매니저(퀘스트 저장 및 관리)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	UQuestManagerComponent* QuestManager;

public:
	//HUD업데이트함수
	UFUNCTION()
	void UpdateHUD();

protected:

	// SceneCapture 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Render")
	USceneCaptureComponent2D* SceneCaptureComponent;

	// RenderTarget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
	UTextureRenderTarget2D* RenderTarget;
#pragma endregion
};