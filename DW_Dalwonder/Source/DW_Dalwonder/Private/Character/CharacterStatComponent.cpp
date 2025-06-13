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
		
			Health = FMath::Clamp(Health - ConsumeRate, 0.f, BaseMaxHealth + BonusMaxHealth);
		}), 0.5f, true);
}

void UCharacterStatComponent::ConsumeStamina(float ConsumeRate)
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaTimer, FTimerDelegate::CreateLambda([&]
		{
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Stamina : %f"), GetStamina()));
		
			if (FMath::IsNearlyZero(Stamina) || Character->CurrentCombatState == ECharacterCombatState::Dead)
			{
				StopConsumeStamina();
			}
		
			Stamina = FMath::Clamp(Stamina - ConsumeRate, 0.f, BaseMaxStamina + BonusMaxStamina);
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
			if (Character->CurrentCombatState == ECharacterCombatState::Dead)
			{
				StopConsumeHealth();
			}
		
			if (FMath::IsNearlyEqual(Health, BaseMaxHealth + BonusMaxHealth))
			{
				StopConsumeHealth();
			}
		
			Health = FMath::Clamp(Health + BaseHealthGenRate + BonusHealthGenRate, 0.f, BaseMaxHealth + BonusMaxHealth);
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
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Stamina : %f"), GetStamina()));

			if (Character->CurrentCombatState == ECharacterCombatState::Dead)
			{
				StopConsumeHealth();
			}
		
			if (FMath::IsNearlyEqual(Stamina, BaseMaxStamina + BonusMaxStamina))
			{
				StopConsumeStamina();
			}
		
			Stamina = FMath::Clamp(Stamina + BaseStaminaGenRate + BonusStaminaGenRate, 0.f, BaseMaxStamina + BonusMaxStamina);
		}), 0.5f, true);
}

void UCharacterStatComponent::SetHealth(const float Value)
{
	Health = FMath::Clamp(Value, 0.0f, BaseMaxHealth + BonusMaxHealth);

	if (Health < BaseMaxHealth + BonusMaxHealth)
	{
		GenHealth();
	}
}

void UCharacterStatComponent::SetBaseMaxHealth(const float Value)
{
	BaseMaxHealth = Value;

	if (Health < BaseMaxHealth + BonusMaxHealth)
	{
		GenHealth();
	}
}

void UCharacterStatComponent::SetBonusMaxHealth(const float Value)
{
	BonusMaxHealth = Value;

	if (Health < BaseMaxHealth + BonusMaxHealth)
	{
		GenHealth();
	}
}

void UCharacterStatComponent::SetStamina(const float Value)
{
	Stamina = FMath::Clamp(Value, 0.0f, BaseMaxStamina + BonusMaxStamina);

	if (Stamina < BaseMaxStamina + BonusMaxStamina)
	{
		GenStamina();
	}
}

void UCharacterStatComponent::SetBaseMaxStamina(const float Value)
{
	BaseMaxStamina = Value;

	if (Stamina < BaseMaxStamina + BonusMaxStamina)
	{
		GenStamina();
	}
}

void UCharacterStatComponent::SetBonusMaxStamina(const float Value)
{
	BonusMaxStamina = Value;

	if (Stamina < BaseMaxStamina + BonusMaxStamina)
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
