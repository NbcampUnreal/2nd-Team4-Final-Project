#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DW_DALWONDER_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetHealth() const
	{
		return Health;
	}

	void SetHealth(const float Value)
	{
		Health = FMath::Clamp(Value, 0.0f, MaxHealth);
	}

	float GetMaxHealth() const
	{
		return MaxHealth;
	}

	void SetMaxHealth(const float Value)
	{
		MaxHealth = Value;
	}

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

	void SetStamina(const float Value)
	{
		Stamina = FMath::Clamp(Value, 0.0f, MaxStamina);
	}

	float GetMaxStamina() const
	{
		return MaxStamina;
	}

	void SetMaxStamina(const float Value)
	{
		MaxStamina = Value;
	}

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

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Health = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float HealthGenRate = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Stamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxStamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float StaminaGenRate = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Attack = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float Defense = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float StaminaConsumption = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float WalkSpeed = 300.f;
};
