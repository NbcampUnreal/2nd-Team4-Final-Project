#include "Character/CharacterStatComponent.h"
#include "Character/DW_CharacterBase.h"

UCharacterStatComponent::UCharacterStatComponent()
{
	Character = Cast<ADW_CharacterBase>(GetOwner());
}

void UCharacterStatComponent::ConsumeHealth(float ConsumeRate)
{
	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
	GetWorld()->GetTimerManager().SetTimer(HealthTimer, FTimerDelegate::CreateLambda([&]
		{
			if (FMath::IsNearlyZero(Health) || Character->CurrentCombatState == ECharacterCombatState::Dead)
			{
				StopConsumeHealth();
			}
		
			Health = FMath::Clamp(Health - ConsumeRate, 0.f, MaxHealth);
		}), 0.5f, true);
}

void UCharacterStatComponent::ConsumeStamina(float ConsumeRate)
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaTimer, FTimerDelegate::CreateLambda([&]
		{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Stamina : %f"), GetStamina()));
		
			if (FMath::IsNearlyZero(Stamina) || Character->CurrentCombatState == ECharacterCombatState::Dead)
			{
				StopConsumeStamina();
			}
		
			Stamina = FMath::Clamp(Stamina - ConsumeRate, 0.f, MaxStamina);
		}), 0.5f, true);
}

void UCharacterStatComponent::StopConsumeHealth()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
}

void UCharacterStatComponent::StopConsumeStamina()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);

	if (Character->bIsGuarding)
	{
		Character->SetGuarding(false);
	}

	if (Character->bIsSprinting)
	{
		Character->Sprint(false);
	}
}

void UCharacterStatComponent::GenHealth()
{
	if (Character->CurrentCombatState == ECharacterCombatState::Dead)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
	GetWorld()->GetTimerManager().SetTimer(HealthTimer, FTimerDelegate::CreateLambda([&]
		{
			if (FMath::IsNearlyEqual(Health, MaxHealth))
			{
				StopConsumeHealth();
			}
		
			Health = FMath::Clamp(Health + HealthGenRate, 0.f, MaxHealth);
		}), 0.5f, true);
}

void UCharacterStatComponent::GenStamina()
{
	if (Character->CurrentCombatState == ECharacterCombatState::Dead)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaTimer, FTimerDelegate::CreateLambda([&]
		{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Stamina : %f"), GetStamina()));
		
			if (FMath::IsNearlyEqual(Stamina, MaxStamina))
			{
				StopConsumeStamina();
			}
		
			Stamina = FMath::Clamp(Stamina + StaminaGenRate, 0.f, MaxStamina);
		}), 0.5f, true);
}

void UCharacterStatComponent::SetHealth(const float Value)
{
	Health = FMath::Clamp(Value, 0.0f, MaxHealth);

	if (Health < MaxHealth)
	{
		GenHealth();
	}
}

void UCharacterStatComponent::SetMaxHealth(const float Value)
{
	MaxHealth = Value;

	if (Health < MaxHealth)
	{
		GenHealth();
	}
}

void UCharacterStatComponent::SetStamina(const float Value)
{
	Stamina = FMath::Clamp(Value, 0.0f, MaxStamina);

	if (Stamina < MaxStamina)
	{
		GenStamina();
	}
}

void UCharacterStatComponent::SetMaxStamina(const float Value)
{
	MaxStamina = Value;

	if (Stamina < MaxStamina)
	{
		GenStamina();
	}
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
