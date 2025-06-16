#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

class ADW_CharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DW_DALWONDER_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStatComponent();

	void ConsumeHealth(float ConsumeRate);

	void ConsumeStamina(float ConsumeRate);

	void StopConsumeHealth();

	void StopConsumeStamina();

	void GenHealth();

	void GenStamina();

#pragma region GetterSetter
	float GetHealth() const
	{
		return Health;
	}

	void SetHealth(const float Value);
	
	float GetBaseMaxHealth() const
	{
		return BaseMaxHealth;
	}

	void SetBaseMaxHealth(const float Value);

	float GetBonusMaxHealth() const
	{
		return BonusMaxHealth;
	}

	void SetBonusMaxHealth(const float Value);
	
	float GetBaseHealthGenRate() const
	{
		return BaseHealthGenRate;
	}

	void SetBaseHealthGenRate(const float Value)
	{
		BaseHealthGenRate = Value;
	}

	float GetBonusHealthGenRate() const
	{
		return BonusHealthGenRate;
	}

	void SetBonusHealthGenRate(const float Value)
	{
		BonusHealthGenRate = Value;
	}

	float GetStamina() const
	{
		return Stamina;
	}

	void SetStamina(const float Value);

	float GetBaseMaxStamina() const
	{
		return BaseMaxStamina;
	}

	void SetBaseMaxStamina(const float Value);

	float GetBonusMaxStamina() const
	{
		return BonusMaxStamina;
	}

	void SetBonusMaxStamina(const float Value);

	float GetBaseStaminaGenRate() const
	{
		return BaseStaminaGenRate;
	}

	void SetBaseStaminaGenRate(const float Value)
	{
		BaseStaminaGenRate = Value;
	}

	float GetBonusStaminaGenRate() const
	{
		return BonusStaminaGenRate;
	}

	void SetBonusStaminaGenRate(const float Value)
	{
		BonusStaminaGenRate = Value;
	}

	float GetBaseAttack() const
	{
		return BaseAttack;
	}

	void SetBaseAttack(const float Value)
	{
		BaseAttack = Value;
	}

	float GetBonusAttack() const
	{
		return BonusAttack;
	}

	void SetBonusAttack(const float Value)
	{
		BonusAttack = Value;
	}

	float GetBaseDefense() const
	{
		return BaseDefense;
	}

	void SetBaseDefense(const float Value)
	{
		BaseDefense = Value;
	}

	float GetBonusDefense() const
	{
		return BonusDefense;
	}

	void SetBonusDefense(const float Value)
	{
		BonusDefense = Value;
	}
	
	float GetWeight() const
	{
		return Weight;
	}

	void SetWeight(const float Value)
	{
		Weight = FMath::Clamp(Value, 0.f, BaseMaxWeight + BonusMaxWeight);
	}

	float GetBaseMaxWeight() const
	{
		return BaseMaxWeight;
	}

	void SetBaseMaxWeight(const float Value)
	{
		BaseMaxWeight = Value;
	}

	float GetBonusMaxWeight() const
	{
		return BonusMaxWeight;
	}

	void SetBonusMaxWeight(const float Value)
	{
		BonusMaxWeight = Value;
	}

	float GetBaseAttackSpeed() const
	{
		return BaseAttackSpeed;
	}

	void SetBaseAttackSpeed(const float Value)
	{
		BaseAttackSpeed = Value;
	}

	float GetBonusAttackSpeed() const
	{
		return BonusAttackSpeed;
	}

	void SetBonusAttackSpeed(const float Value)
	{
		BonusAttackSpeed = Value;
	}

	float GetBaseWalkSpeed() const
	{
		return BaseWalkSpeed;
	}

	void SetBaseWalkSpeed(const float Value)
	{
		BaseWalkSpeed = Value;
	}

	float GetBonusWalkSpeed() const
	{
		return BonusWalkSpeed;
	}

	void SetBonusWalkSpeed(const float Value)
	{
		BonusWalkSpeed = Value;
	}

	float GetSprintSpeed() const
	{
		return SprintSpeed;
	}
#pragma endregion
	
protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Health = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BaseMaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BonusMaxHealth = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BaseHealthGenRate = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BonusHealthGenRate = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Stamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BaseMaxStamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BonusMaxStamina = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BaseStaminaGenRate = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BonusStaminaGenRate = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BaseAttack = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BonusAttack = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BaseDefense = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BonusDefense = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Weight = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BaseMaxWeight = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BonusMaxWeight = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BaseAttackSpeed = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BonusAttackSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BaseWalkSpeed = 300.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float BonusWalkSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float SprintSpeed = 2 * (BaseWalkSpeed + BonusWalkSpeed);

	UPROPERTY()
	FTimerHandle HealthTimer;

	UPROPERTY()
	FTimerHandle StaminaTimer;

	UPROPERTY()
	ADW_CharacterBase* Character;
};
