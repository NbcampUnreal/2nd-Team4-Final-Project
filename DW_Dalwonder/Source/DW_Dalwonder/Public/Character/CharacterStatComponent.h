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
	
	float GetMaxHealth() const
	{
		return MaxHealth;
	}

	void SetMaxHealth(const float Value);
	
	float GetHealthGenRate() const
	{
		return HealthGenRate;
	}

	void SetHealthGenRate(const float Value)
	{
		HealthGenRate = Value;
	}

	float GetStamina() const
	{
		return Stamina;
	}

	void SetStamina(const float Value);

	float GetMaxStamina() const
	{
		return MaxStamina;
	}

	void SetMaxStamina(const float Value);

	float GetStaminaGenRate() const
	{
		return StaminaGenRate;
	}

	void SetStaminaGenRate(const float Value)
	{
		StaminaGenRate = Value;
	}

	float GetAttack() const
	{
		return Attack;
	}

	void SetAttack(const float Value)
	{
		Attack = Value;
	}

	float GetDefense() const
	{
		return Defense;
	}

	void SetDefense(const float Value)
	{
		Defense = Value;
	}
	
	float GetWeight() const
	{
		return Weight;
	}

	void SetWeight(const float Value)
	{
		Weight = FMath::Clamp(Value, 0.f, MaxWeight);
	}

	float GetMaxWeight() const
	{
		return MaxWeight;
	}

	void SetMaxWeight(const float Value)
	{
		MaxWeight = Value;
	}

	float GetStaminaConsumption() const
	{
		return StaminaConsumption;
	}

	void SetStaminaConsumption(const float Value)
	{
		StaminaConsumption = Value;
	}

	float GetWalkSpeed() const
	{
		return WalkSpeed;
	}

	void SetWalkSpeed(const float Value)
	{
		WalkSpeed = Value;
	}

	float GetSprintSpeed() const
	{
		return SprintSpeed;
	}

	void SetSprintSpeed(const float Value)
	{
		SprintSpeed = Value;
	}
#pragma endregion
	
protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Health = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float HealthGenRate = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Stamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxStamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float StaminaGenRate = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Attack = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Defense = 10.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Weight = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxWeight = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float StaminaConsumption = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float WalkSpeed = 300.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float SprintSpeed = 600.f;

	UPROPERTY()
	FTimerHandle HealthTimer;

	UPROPERTY()
	FTimerHandle StaminaTimer;

	UPROPERTY()
	ADW_CharacterBase* Character;
};
