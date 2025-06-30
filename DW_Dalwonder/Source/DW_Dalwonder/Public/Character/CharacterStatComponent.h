#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "Item/ItemData.h"
#include "EAttrType.h"
#include "CharacterStatComponent.generated.h"

// 스탯 변경을 알리는 델리게이트 (UI 업데이트 등에 사용)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChangedSignature, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllStatsRecalculatedSignature); // 모든 총합 스탯이 변경될 때

class ADW_CharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DW_DALWONDER_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// --- 주요 스탯 변경 함수 ---
	void ConsumeHealth(float ConsumeRate);
	void ConsumeStamina(float ConsumeRate);
	void StopConsumeHealth();
	void StopConsumeStamina();
	void StartHealthRegen();
	void StartStaminaRegen();

	// 모든 총합 스탯을 다시 계산하는 함수
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void RecalculateAllTotalStats();

	// 특정 스탯의 총합을 다시 계산하는 함수들
	void RecalculateTotalMaxHealth();
	void RecalculateTotalHealthGenRate();
	void RecalculateTotalMaxStamina();
	void RecalculateTotalStaminaGenRate();
	void RecalculateTotalAttack();
	void RecalculateTotalDefense();
	void RecalculateTotalAttackSpeed();
	void RecalculateTotalWalkSpeed();
	void RecalculateTotalMaxWeight();

	// 버프 적용 함수
	UFUNCTION(BlueprintCallable, Category = "Stat | Buffs")
	void ApplyStatBuff(EConsumableEffectType EffectType, float Amount, float Duration);

	// 속성 보너스 관련 함수
	void SetAttrBonus(EAttrType Type, float Bonus);

protected:
	void RemoveStatBuff(EConsumableEffectType EffectType, float Amount);

	// 모든 버프 타이머를 클리어하는 함수
	void ClearAllBuffTimers();

public:

	// --- 델리게이트 (UI 업데이트 용) ---
	UPROPERTY(BlueprintAssignable, Category = "Stat | Events")
	FOnStatChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stat | Events")
	FOnStatChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stat | Events")
	FOnAllStatsRecalculatedSignature OnTotalStatsRecalculated; // 모든 총합 스탯 변경 시

#pragma region GetterSetter
	// --- 현재 체력/스태미나 ---
	UFUNCTION(BlueprintPure, Category = "Stat | Current")
	float GetHealth() const { return Health; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Current")
	void SetHealth(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Current")
	float GetStamina() const { return Stamina; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Current")
	void SetStamina(const float Value);


	// --- 총합 스탯 Getter ---
	UFUNCTION(BlueprintPure, Category = "Stat | Total")
	float GetTotalMaxHealth() const { return TotalMaxHealth; }
	UFUNCTION(BlueprintPure, Category = "Stat | Total")
	float GetTotalHealthGenRate() const { return TotalHealthGenRate; }
	UFUNCTION(BlueprintPure, Category = "Stat | Total")
	float GetTotalMaxStamina() const { return TotalMaxStamina; }
	UFUNCTION(BlueprintPure, Category = "Stat | Total")
	float GetTotalStaminaGenRate() const { return TotalStaminaGenRate; }
	UFUNCTION(BlueprintPure, Category = "Stat | Total")
	float GetTotalAttack() const { return TotalAttack; }
	UFUNCTION(BlueprintPure, Category = "Stat | Total")
	float GetTotalDefense() const { return TotalDefense; }
	UFUNCTION(BlueprintPure, Category = "Stat | Total")
	float GetTotalAttackSpeed() const { return TotalAttackSpeed; }
	UFUNCTION(BlueprintPure, Category = "Stat | Total")
	float GetTotalWalkSpeed() const { return TotalWalkSpeed; }
	UFUNCTION(BlueprintPure, Category = "Stat | Total")
	float GetTotalMaxWeight() const { return TotalMaxWeight; }
	UFUNCTION(BlueprintPure, Category = "Stat | Total")
	float GetSprintSpeed() const { return GetTotalWalkSpeed() * 2.0f; } // TotalWalkSpeed에 기반하여 계산

	UFUNCTION(BlueprintPure, Category = "Stat | Current")
	float GetCurrentWeight() const { return CurrentWeight; } // 현재 무게 getter

	UFUNCTION(BlueprintCallable, Category = "Stat | Current")
	void SetCurrentWeight(const float Value); // 현재 무게 setter (TotalMaxWeight에 클램프)


	// --- Base 스탯 Getter/Setter ---
	UFUNCTION(BlueprintPure, Category = "Stat | Base")
	float GetBaseMaxHealth() const { return BaseMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Base")
	void SetBaseMaxHealth(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Base")
	float GetBaseHealthGenRate() const { return BaseHealthGenRate; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Base")
	void SetBaseHealthGenRate(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Base")
	float GetBaseMaxStamina() const { return BaseMaxStamina; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Base")
	void SetBaseMaxStamina(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Base")
	float GetBaseStaminaGenRate() const { return BaseStaminaGenRate; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Base")
	void SetBaseStaminaGenRate(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Base")
	float GetBaseAttack() const { return BaseAttack; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Base")
	void SetBaseAttack(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Base")
	float GetBaseDefense() const { return BaseDefense; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Base")
	void SetBaseDefense(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Base")
	float GetBaseMaxWeight() const { return BaseMaxWeight; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Base")
	void SetBaseMaxWeight(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Base")
	float GetBaseAttackSpeed() const { return BaseAttackSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Base")
	void SetBaseAttackSpeed(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Base")
	float GetBaseWalkSpeed() const { return BaseWalkSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Base")
	void SetBaseWalkSpeed(const float Value);


	// --- BuffBonus 스탯 Getter/Setter (ApplyStatBuff/RemoveStatBuff에서 내부적으로 사용) ---
	UFUNCTION(BlueprintPure, Category = "Stat | Buff Bonus")
	float GetBuffBonusMaxHealth() const { return BuffBonusMaxHealth; }
	UFUNCTION(BlueprintPure, Category = "Stat | Buff Bonus")
	float GetBuffBonusHealthGenRate() const { return BuffBonusHealthGenRate; }
	UFUNCTION(BlueprintPure, Category = "Stat | Buff Bonus")
	float GetBuffBonusMaxStamina() const { return BuffBonusMaxStamina; }
	UFUNCTION(BlueprintPure, Category = "Stat | Buff Bonus")
	float GetBuffBonusStaminaGenRate() const { return BuffBonusStaminaGenRate; }
	UFUNCTION(BlueprintPure, Category = "Stat | Buff Bonus")
	float GetBuffBonusAttack() const { return BuffBonusAttack; }
	UFUNCTION(BlueprintPure, Category = "Stat | Buff Bonus")
	float GetBuffBonusDefense() const { return BuffBonusDefense; }
	UFUNCTION(BlueprintPure, Category = "Stat | Buff Bonus")
	float GetBuffBonusMaxWeight() const { return BuffBonusMaxWeight; }
	UFUNCTION(BlueprintPure, Category = "Stat | Buff Bonus")
	float GetBuffBonusAttackSpeed() const { return BuffBonusAttackSpeed; }
	UFUNCTION(BlueprintPure, Category = "Stat | Buff Bonus")
	float GetBuffBonusWalkSpeed() const { return BuffBonusWalkSpeed; }


	// --- EquipmentBonus 스탯 Getter/Setter (InventoryComponent에서 사용) ---
	UFUNCTION(BlueprintPure, Category = "Stat | Equipment Bonus")
	float GetEquipmentBonusMaxHealth() const { return EquipmentBonusMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void SetEquipmentBonusMaxHealth(const float Value);
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void AddEquipmentBonusMaxHealth(const float Value); // 누적을 위한 Add 함수

	UFUNCTION(BlueprintPure, Category = "Stat | Equipment Bonus")
	float GetEquipmentBonusHealthGenRate() const { return EquipmentBonusHealthGenRate; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void SetEquipmentBonusHealthGenRate(const float Value);
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void AddEquipmentBonusHealthGenRate(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Equipment Bonus")
	float GetEquipmentBonusMaxStamina() const { return EquipmentBonusMaxStamina; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void SetEquipmentBonusMaxStamina(const float Value);
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void AddEquipmentBonusMaxStamina(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Equipment Bonus")
	float GetEquipmentBonusStaminaGenRate() const { return EquipmentBonusStaminaGenRate; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void SetEquipmentBonusStaminaGenRate(const float Value);
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void AddEquipmentBonusStaminaGenRate(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Equipment Bonus")
	float GetEquipmentBonusAttack() const { return EquipmentBonusAttack; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void SetEquipmentBonusAttack(const float Value);
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void AddEquipmentBonusAttack(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Equipment Bonus")
	float GetEquipmentBonusDefense() const { return EquipmentBonusDefense; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void SetEquipmentBonusDefense(const float Value);
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void AddEquipmentBonusDefense(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Equipment Bonus")
	float GetEquipmentBonusMaxWeight() const { return EquipmentBonusMaxWeight; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void SetEquipmentBonusMaxWeight(const float Value);
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void AddEquipmentBonusMaxWeight(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Equipment Bonus")
	float GetEquipmentBonusAttackSpeed() const { return EquipmentBonusAttackSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void SetEquipmentBonusAttackSpeed(const float Value);
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void AddEquipmentBonusAttackSpeed(const float Value);

	UFUNCTION(BlueprintPure, Category = "Stat | Equipment Bonus")
	float GetEquipmentBonusWalkSpeed() const { return EquipmentBonusWalkSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void SetEquipmentBonusWalkSpeed(const float Value);
	UFUNCTION(BlueprintCallable, Category = "Stat | Equipment Bonus")
	void AddEquipmentBonusWalkSpeed(const float Value);

#pragma endregion

protected:
	// --- 현재 스탯 값 (실제 사용되는 값) ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Current")
	float Health = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Current")
	float Stamina = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Current")
	float CurrentWeight = 0.f; // 현재 소지 무게

	// --- 총합 스탯 (Base + EquipmentBonus + BuffBonus) ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Total")
	float TotalMaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Total")
	float TotalHealthGenRate = 0.5f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Total")
	float TotalMaxStamina = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Total")
	float TotalStaminaGenRate = 0.5f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Total")
	float TotalAttack = 10.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Total")
	float TotalDefense = 10.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Total")
	float TotalAttackSpeed = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Total")
	float TotalWalkSpeed = 300.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Total")
	float TotalMaxWeight = 100.f;


	// --- Base 스탯 (레벨업 등으로 영구히 증가) ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat | Base")
	float BaseMaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat | Base")
	float BaseHealthGenRate = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat | Base")
	float BaseMaxStamina = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat | Base")
	float BaseStaminaGenRate = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat | Base")
	float BaseAttack = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat | Base")
	float BaseDefense = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat | Base")
	float BaseMaxWeight = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat | Base")
	float BaseAttackSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat | Base")
	float BaseWalkSpeed = 300.f;


	// --- Buff 보너스 스탯 (소모품/스킬 등 일시적 버프) ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Buff Bonus")
	float BuffBonusMaxHealth = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Buff Bonus")
	float BuffBonusHealthGenRate = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Buff Bonus")
	float BuffBonusMaxStamina = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Buff Bonus")
	float BuffBonusStaminaGenRate = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Buff Bonus")
	float BuffBonusAttack = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Buff Bonus")
	float BuffBonusDefense = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Buff Bonus")
	float BuffBonusMaxWeight = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Buff Bonus")
	float BuffBonusAttackSpeed = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Buff Bonus")
	float BuffBonusWalkSpeed = 0.f;


	// --- Equipment 보너스 스탯 (장비에 의한 보너스) ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Equipment Bonus")
	float EquipmentBonusMaxHealth = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Equipment Bonus")
	float EquipmentBonusHealthGenRate = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Equipment Bonus")
	float EquipmentBonusMaxStamina = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Equipment Bonus")
	float EquipmentBonusStaminaGenRate = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Equipment Bonus")
	float EquipmentBonusAttack = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Equipment Bonus")
	float EquipmentBonusDefense = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Equipment Bonus")
	float EquipmentBonusMaxWeight = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Equipment Bonus")
	float EquipmentBonusAttackSpeed = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Equipment Bonus")
	float EquipmentBonusWalkSpeed = 0.f;

	// --- 속성 보너스 스탯 (속성 시스템에 의한 보너스) ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttrBonus")
	float AttrBonusMaxStamina = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttrBonus")
	float AttrBonusMaxHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttrBonus")
	float AttrBonusDefense = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttrBonus")
	float AttrBonusStaminaRegen = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttrBonus")
	float AttrBonusHealthRegen = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttrBonus")
	float AttrBonusMaxCarryWeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttrBonus")
	float AttrBonusMoveSpeed = 0.f;

	// --- 타이머 핸들 및 관련 멤버 변수 ---
	UPROPERTY()
	FTimerHandle HealthTimer;
	UPROPERTY()
	FTimerHandle StaminaTimer;

	// 각 버프 타입별 타이머 핸들을 관리하기 위한 TMap
	UPROPERTY()
	TMap<EConsumableEffectType, FTimerHandle> BuffTimerHandlesMap;

	UPROPERTY()
	ADW_CharacterBase* Character;

	/*UPROPERTY()
	class UDW_AttributeComponent* Attr;*/
};
