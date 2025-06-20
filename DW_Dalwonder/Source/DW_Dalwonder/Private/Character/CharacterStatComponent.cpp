#include "Character/CharacterStatComponent.h"
#include "Character/DW_CharacterBase.h"

UCharacterStatComponent::UCharacterStatComponent()
{
	Character = nullptr;
}

void UCharacterStatComponent::ConsumeHealth(float ConsumeRate)
{
	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
	GetWorld()->GetTimerManager().SetTimer(HealthTimer, FTimerDelegate::CreateLambda([&]
		{
			if (FMath::IsNearlyZero(Health) || Character->CurrentCombatState == ECharacterCombatState::Dead)
			{
				StopConsumeHealth();
				return;
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
				return;
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

void UCharacterStatComponent::StartHealthRegen()
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
				return;
			}
		
			if (Health >= BaseMaxHealth + BonusMaxHealth)
			{
				StopConsumeHealth();
				return;
			}
		
			Health = FMath::Clamp(Health + BaseHealthGenRate + BonusHealthGenRate, 0.f, BaseMaxHealth + BonusMaxHealth);
		}), 0.5f, true);
}

void UCharacterStatComponent::StartStaminaRegen()
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
				StopConsumeStamina();
				return;
			}
		
			if (Stamina >= BaseMaxStamina + BonusMaxStamina)
			{
				StopConsumeStamina();
				return;
			}
		
			Stamina = FMath::Clamp(Stamina + BaseStaminaGenRate + BonusStaminaGenRate, 0.f, BaseMaxStamina + BonusMaxStamina);
		}), 0.5f, true);
}

void UCharacterStatComponent::SetHealth(const float Value)
{
	Health = FMath::Clamp(Value, 0.0f, BaseMaxHealth + BonusMaxHealth);

	if (Health < BaseMaxHealth + BonusMaxHealth)
	{
		StartHealthRegen();
	}
}

void UCharacterStatComponent::SetBaseMaxHealth(const float Value)
{
	BaseMaxHealth = Value;

	SetHealth(Health);
}

void UCharacterStatComponent::SetBonusMaxHealth(const float Value)
{
	BonusMaxHealth = Value;

	SetHealth(Health);
}

void UCharacterStatComponent::SetStamina(const float Value)
{
	Stamina = FMath::Clamp(Value, 0.0f, BaseMaxStamina + BonusMaxStamina);

	if (Stamina < BaseMaxStamina + BonusMaxStamina)
	{
		StartStaminaRegen();
	}
}

void UCharacterStatComponent::SetBaseMaxStamina(const float Value)
{
	BaseMaxStamina = Value;

	SetStamina(Stamina);
}

void UCharacterStatComponent::SetBonusMaxStamina(const float Value)
{
	BonusMaxStamina = Value;

	SetStamina(Stamina);
}

void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ADW_CharacterBase>(GetOwner());
}

void UCharacterStatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
	HealthTimer.Invalidate();
	StaminaTimer.Invalidate();

	// 모든 버프 타이머도 클리어
	for (FTimerHandle Handle : BuffTimerHandles)
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);
	}
	BuffTimerHandles.Empty();
}

void UCharacterStatComponent::ApplyAttackBuff(float Amount, float Duration)
{
	SetBonusAttack(GetBonusAttack() + Amount); // 보너스 공격력 증가

	if (Duration > 0.0f)
	{
		FTimerHandle BuffTimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this, Amount]()
			{
				RemoveAttackBuff(Amount);
			});
		GetWorld()->GetTimerManager().SetTimer(BuffTimerHandle, TimerDelegate, Duration, false);
		BuffTimerHandles.Add(BuffTimerHandle); // 타이머 핸들 저장
	}
}

void UCharacterStatComponent::RemoveAttackBuff(float Amount)
{
	SetBonusAttack(GetBonusAttack() - Amount); // 보너스 공격력 감소
	// TODO: BuffTimerHandles에서 해당 타이머 핸들을 제거하는 로직 추가 (복잡하면 생략 가능)
}

void UCharacterStatComponent::ApplyDefenseBuff(float Amount, float Duration)
{
	SetBonusDefense(GetBonusDefense() + Amount); // 보너스 방어력 증가

	if (Duration > 0.0f)
	{
		FTimerHandle BuffTimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this, Amount]()
			{
				RemoveDefenseBuff(Amount);
			});
		GetWorld()->GetTimerManager().SetTimer(BuffTimerHandle, TimerDelegate, Duration, false);
		BuffTimerHandles.Add(BuffTimerHandle);
	}
}

void UCharacterStatComponent::RemoveDefenseBuff(float Amount)
{
	SetBonusDefense(GetBonusDefense() - Amount); // 보너스 방어력 감소
	// TODO: BuffTimerHandles에서 해당 타이머 핸들을 제거하는 로직 추가
}