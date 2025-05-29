#include "Character/CharacterStatComponent.h"

UCharacterStatComponent::UCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

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

