#include "Character/CharacterStatComponent.h"

UCharacterStatComponent::UCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCharacterStatComponent::ConsumeHealth(float ConsumeRate)
{
	bEnableHealthGen = false;
	
	GetWorld()->GetTimerManager().SetTimer(HealthTimer, FTimerDelegate::CreateLambda([&]
		{
			if (FMath::IsNearlyZero(Health))
			{
				StopConsumeHealth();
			}
		
			Health = FMath::Clamp(Health - ConsumeRate, 0.f, MaxHealth);
		}), 0.5f, true);
}

void UCharacterStatComponent::ConsumeStamina(float ConsumeRate)
{
	bEnableStaminaGen = false;
	
	GetWorld()->GetTimerManager().SetTimer(StaminaTimer, FTimerDelegate::CreateLambda([&]
		{
			if (FMath::IsNearlyZero(Stamina))
			{
				StopConsumeStamina();
			}
		
			Stamina = FMath::Clamp(Stamina - ConsumeRate, 0.f, MaxStamina);
		}), 0.5f, true);
}

void UCharacterStatComponent::StopConsumeHealth()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
	bEnableHealthGen = true;
}

void UCharacterStatComponent::StopConsumeStamina()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
	bEnableStaminaGen = true;
}

void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCharacterStatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
	HealthTimer.Invalidate();
	StaminaTimer.Invalidate();
}

void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Health < MaxHealth && bEnableHealthGen)
	{
		SetHealth(Health + HealthGenRate);
	}

	if (Stamina < MaxStamina && bEnableStaminaGen)
	{
		SetStamina(Stamina + StaminaGenRate);
	}
}

