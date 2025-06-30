#include "Character/CharacterStatComponent.h"
#include "Character/DW_CharacterBase.h"
#include "CharacterStatSaveData.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DW_GameInstance.h"

UCharacterStatComponent::UCharacterStatComponent()
{
	Character = nullptr;
}

void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ADW_CharacterBase>(GetOwner());

	// 초기 스탯 계산
	RecalculateAllTotalStats();

	// Health와 Stamina를 TotalMaxHealth/TotalMaxStamina로 초기화
    // 초기화 조건 추가 (처음 시작한 경우만)
    if (const auto* GI = Cast<UDW_GameInstance>(GetWorld()->GetGameInstance()))
    {
        if (GI->bIsNewGame)
        {
            Health = TotalMaxHealth;
            Stamina = TotalMaxStamina;
        }
    }

    // UI 업데이트를 위해 초기 값 브로드캐스트
    OnHealthChanged.Broadcast(Health, TotalMaxHealth);
    OnStaminaChanged.Broadcast(Stamina, TotalMaxStamina);
    OnTotalStatsRecalculated.Broadcast();
}

void UCharacterStatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 모든 타이머 클리어
	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
	ClearAllBuffTimers(); // 버프 타이머들도 클리어
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
		
			Health = FMath::Clamp(Health - ConsumeRate, 0.f, GetTotalMaxHealth());
            OnHealthChanged.Broadcast(Health, TotalMaxHealth);
		}), 0.5f, true);
#if WITH_EDITOR
    UE_LOG(LogTemp, Log, TEXT("Health consume started. Rate: %f"), ConsumeRate);
#endif
}

void UCharacterStatComponent::ConsumeStamina(float ConsumeRate)
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaTimer, FTimerDelegate::CreateLambda([&]
		{
			if (FMath::IsNearlyZero(Stamina) || Character->CurrentCombatState == ECharacterCombatState::Dead)
			{
				StopConsumeStamina();
				return;
			}
		
			Stamina = FMath::Clamp(Stamina - ConsumeRate, 0.f, GetTotalMaxStamina());
            OnStaminaChanged.Broadcast(Stamina, TotalMaxStamina);
		}), 0.5f, true);
}

void UCharacterStatComponent::StopConsumeHealth()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
}

void UCharacterStatComponent::StopConsumeStamina()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);

    if (Character) // Character가 유효한지 확인
    {
        if (Character->bIsGuarding)
        {
            Character->SetGuarding(false);
        }

        if (Character->bIsSprinting)
        {
            Character->Sprint(false);
        }
    }

}

void UCharacterStatComponent::StartHealthRegen()
{
	if (Character->CurrentCombatState == ECharacterCombatState::Dead)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);  

    // 이미 최대 체력이면 재생 시작하지 않음
    if (Health >= GetTotalMaxHealth())
    {
        return;
    }

    GetWorld()->GetTimerManager().SetTimer(HealthTimer, FTimerDelegate::CreateLambda([&]
        {
            // 재생 중 캐릭터가 죽으면 중지
            if (Character && Character->CurrentCombatState == ECharacterCombatState::Dead)
            {
                StopConsumeHealth(); // 재생 타이머를 중지
                return;
            }

            // 체력이 최대치에 도달하면 재생 중지
            if (FMath::IsNearlyEqual(Health, GetTotalMaxHealth())) // 부동 소수점 비교는 IsNearlyEqual 사용
            {
                StopConsumeHealth(); // 재생 타이머를 중지
                return;
            }

            // 체력 재생, GetTotalHealthGenRate() 사용
            Health = FMath::Clamp(Health + GetTotalHealthGenRate(), 0.f, GetTotalMaxHealth());
            OnHealthChanged.Broadcast(Health, TotalMaxHealth); // 체력 변경 UI 업데이트
        }), 0.5f, true);
}

void UCharacterStatComponent::StartStaminaRegen()
{
    if (Character && Character->CurrentCombatState == ECharacterCombatState::Dead)
    {
        return;
    }

    // 기존 타이머가 있다면 중지
    GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);

    // 이미 최대 스태미너면 재생 시작하지 않음
    if (Stamina >= GetTotalMaxStamina())
    {
        return;
    }

    GetWorld()->GetTimerManager().SetTimer(StaminaTimer, FTimerDelegate::CreateLambda([&]
        {
            // 재생 중 캐릭터가 죽으면 중지
            if (Character && Character->CurrentCombatState == ECharacterCombatState::Dead)
            {
                StopConsumeStamina(); // 재생 타이머를 중지
                return;
            }

            // 스태미나가 최대치에 도달하면 재생 중지
            if (FMath::IsNearlyEqual(Stamina, GetTotalMaxStamina()))
            {
                StopConsumeStamina(); // 재생 타이머를 중지
                return;
            }

            // 스태미너 재생, GetTotalStaminaGenRate() 사용
            Stamina = FMath::Clamp(Stamina + GetTotalStaminaGenRate(), 0.f, GetTotalMaxStamina());
            OnStaminaChanged.Broadcast(Stamina, TotalMaxStamina); // 스태미나 변경 UI 업데이트
        }), 0.5f, true);
}

#pragma region GetterSetter
// --- 현재 체력/스태미나 Setter ---
void UCharacterStatComponent::SetHealth(const float Value)
{
    // TotalMaxHealth를 사용
    Health = FMath::Clamp(Value, 0.f, TotalMaxHealth);
    OnHealthChanged.Broadcast(Health, TotalMaxHealth);
}

void UCharacterStatComponent::SetStamina(const float Value)
{
    // TotalMaxStamina를 사용
    Stamina = FMath::Clamp(Value, 0.f, TotalMaxStamina);
    OnStaminaChanged.Broadcast(Stamina, TotalMaxStamina);
}

void UCharacterStatComponent::SetCurrentWeight(const float Value)
{
    CurrentWeight = FMath::Clamp(Value, 0.f, TotalMaxWeight);
}


// --- Base 스탯 Setter ---
void UCharacterStatComponent::SetBaseMaxHealth(const float Value)
{
    BaseMaxHealth = Value;
    RecalculateTotalMaxHealth(); 
}
void UCharacterStatComponent::SetBaseHealthGenRate(const float Value)
{
    BaseHealthGenRate = Value;
    RecalculateTotalHealthGenRate(); // 구현 예정
}
void UCharacterStatComponent::SetBaseMaxStamina(const float Value)
{
	BaseMaxStamina = Value;
	RecalculateTotalMaxStamina(); 
}
void UCharacterStatComponent::SetBaseStaminaGenRate(const float Value)
{
	BaseStaminaGenRate = Value;
	RecalculateTotalStaminaGenRate(); // 구현 예정
}
void UCharacterStatComponent::SetBaseAttack(const float Value)
{
	BaseAttack = Value;
	RecalculateTotalAttack(); 
}
void UCharacterStatComponent::SetBaseDefense(const float Value)
{
	BaseDefense = Value;
	RecalculateTotalDefense(); 
}
void UCharacterStatComponent::SetBaseMaxWeight(const float Value)
{
	BaseMaxWeight = Value;
	RecalculateTotalMaxWeight(); 
}
void UCharacterStatComponent::SetBaseAttackSpeed(const float Value)
{
	BaseAttackSpeed = Value;
	RecalculateTotalAttackSpeed(); 
}
void UCharacterStatComponent::SetBaseWalkSpeed(const float Value)
{
	BaseWalkSpeed = Value;
	RecalculateTotalWalkSpeed(); 
}


// --- EquipmentBonus 스탯 Setter ---
void UCharacterStatComponent::SetEquipmentBonusMaxHealth(const float Value)
{
    EquipmentBonusMaxHealth = Value;
    RecalculateTotalMaxHealth();
}
void UCharacterStatComponent::AddEquipmentBonusMaxHealth(const float Value)
{
    EquipmentBonusMaxHealth += Value;
    RecalculateTotalMaxHealth();
}
void UCharacterStatComponent::SetEquipmentBonusHealthGenRate(const float Value)
{
	EquipmentBonusHealthGenRate = Value;
	RecalculateTotalHealthGenRate();
}
void UCharacterStatComponent::AddEquipmentBonusHealthGenRate(const float Value)
{
	EquipmentBonusHealthGenRate += Value;
	RecalculateTotalHealthGenRate();
}
void UCharacterStatComponent::SetEquipmentBonusMaxStamina(const float Value)
{
	EquipmentBonusMaxStamina = Value;
	RecalculateTotalMaxStamina();
}
void UCharacterStatComponent::AddEquipmentBonusMaxStamina(const float Value)
{
	EquipmentBonusMaxStamina += Value;
	RecalculateTotalMaxStamina();
}
void UCharacterStatComponent::SetEquipmentBonusStaminaGenRate(const float Value)
{
	EquipmentBonusStaminaGenRate = Value;
	RecalculateTotalStaminaGenRate();
}
void UCharacterStatComponent::AddEquipmentBonusStaminaGenRate(const float Value)
{
	EquipmentBonusStaminaGenRate += Value;
	RecalculateTotalStaminaGenRate();
}
void UCharacterStatComponent::SetEquipmentBonusAttack(const float Value)
{
	EquipmentBonusAttack = Value;
	RecalculateTotalAttack();
}
void UCharacterStatComponent::AddEquipmentBonusAttack(const float Value)
{
	EquipmentBonusAttack += Value;
	RecalculateTotalAttack();
}
void UCharacterStatComponent::SetEquipmentBonusDefense(const float Value)
{
	EquipmentBonusDefense = Value;
	RecalculateTotalDefense();
}
void UCharacterStatComponent::AddEquipmentBonusDefense(const float Value)
{
	EquipmentBonusDefense += Value;
	RecalculateTotalDefense();
}
void UCharacterStatComponent::SetEquipmentBonusMaxWeight(const float Value)
{
	EquipmentBonusMaxWeight = Value;
	RecalculateTotalMaxWeight();
}
void UCharacterStatComponent::AddEquipmentBonusMaxWeight(const float Value)
{
	EquipmentBonusMaxWeight += Value;
	RecalculateTotalMaxWeight();
}
void UCharacterStatComponent::SetEquipmentBonusAttackSpeed(const float Value)
{
	EquipmentBonusAttackSpeed = Value;
	RecalculateTotalAttackSpeed();
}
void UCharacterStatComponent::AddEquipmentBonusAttackSpeed(const float Value)
{
	EquipmentBonusAttackSpeed += Value;
	RecalculateTotalAttackSpeed();
}
void UCharacterStatComponent::SetEquipmentBonusWalkSpeed(const float Value)
{
	EquipmentBonusWalkSpeed = Value;
	RecalculateTotalWalkSpeed();
}
void UCharacterStatComponent::AddEquipmentBonusWalkSpeed(const float Value)
{
	EquipmentBonusWalkSpeed += Value;
	RecalculateTotalWalkSpeed();
}

#pragma endregion

// --- 총합 스탯 재계산 함수들 ---
void UCharacterStatComponent::RecalculateAllTotalStats()
{
    RecalculateTotalMaxHealth();
    RecalculateTotalMaxStamina();
    RecalculateTotalAttack();
    RecalculateTotalDefense();
    RecalculateTotalAttackSpeed();
    RecalculateTotalWalkSpeed();
    RecalculateTotalMaxWeight();
	RecalculateTotalHealthGenRate();
	RecalculateTotalStaminaGenRate();
	// 모든 총합 스탯 변경 알림
	OnTotalStatsRecalculated.Broadcast();
}

void UCharacterStatComponent::RecalculateTotalMaxHealth()
{
    float OldTotalMaxHealth = TotalMaxHealth;
    TotalMaxHealth = BaseMaxHealth + EquipmentBonusMaxHealth + BuffBonusMaxHealth + AttrBonusMaxHealth;

    // 캐릭터가 버프 등으로 최대 체력이 낮아질 때 현재 체력이 갑자기 줄어드는 것을 방지
    SetHealth(Health);

    if (TotalMaxHealth != OldTotalMaxHealth)
    {
        OnHealthChanged.Broadcast(Health, TotalMaxHealth); // 최대 체력 변경 알림
    }
}

void UCharacterStatComponent::RecalculateTotalHealthGenRate()
{
    TotalHealthGenRate = BaseHealthGenRate + EquipmentBonusHealthGenRate + BuffBonusHealthGenRate + AttrBonusHealthRegen;
}

void UCharacterStatComponent::RecalculateTotalMaxStamina()
{
    float OldTotalMaxStamina = TotalMaxStamina;
    TotalMaxStamina = BaseMaxStamina + EquipmentBonusMaxStamina + BuffBonusMaxStamina + AttrBonusMaxStamina;
    SetStamina(Stamina); // SetStamina 내부에서 클램프 처리

    if (TotalMaxStamina != OldTotalMaxStamina)
    {
        OnStaminaChanged.Broadcast(Stamina, TotalMaxStamina);
    }
}

void UCharacterStatComponent::RecalculateTotalStaminaGenRate()
{
	TotalStaminaGenRate = BaseStaminaGenRate + EquipmentBonusStaminaGenRate + BuffBonusStaminaGenRate + AttrBonusStaminaRegen;
}

void UCharacterStatComponent::RecalculateTotalAttack()
{
    TotalAttack = BaseAttack + EquipmentBonusAttack + BuffBonusAttack;
}

void UCharacterStatComponent::RecalculateTotalDefense()
{
	TotalDefense = BaseDefense + EquipmentBonusDefense + BuffBonusDefense + AttrBonusDefense;
}

void UCharacterStatComponent::RecalculateTotalAttackSpeed()
{
    TotalAttackSpeed = BaseAttackSpeed + EquipmentBonusAttackSpeed + BuffBonusAttackSpeed;
}

void UCharacterStatComponent::RecalculateTotalWalkSpeed()
{
	TotalWalkSpeed = BaseWalkSpeed + EquipmentBonusWalkSpeed + BuffBonusWalkSpeed + AttrBonusMoveSpeed;
}

void UCharacterStatComponent::RecalculateTotalMaxWeight()
{
	TotalMaxWeight = BaseMaxWeight + EquipmentBonusMaxWeight + BuffBonusMaxWeight + AttrBonusMaxCarryWeight;
    SetCurrentWeight(CurrentWeight); // 현재 무게를 새로운 최대 무게에 클램프
}

// --- 버프 적용/제거 함수 ---
void UCharacterStatComponent::ApplyStatBuff(EConsumableEffectType EffectType, float Amount, float Duration)
{
    if (EffectType == EConsumableEffectType::HealHealth || EffectType == EConsumableEffectType::RestoreStamina)
    {
        return;
    }

    // 해당 EffectType에 대한 기존 타이머가 있다면 클리어
    if (BuffTimerHandlesMap.Contains(EffectType))
    {
        GetWorld()->GetTimerManager().ClearTimer(BuffTimerHandlesMap[EffectType]);
        BuffTimerHandlesMap.Remove(EffectType);
    }

    // 스탯에 Amount 추가
    switch (EffectType)
    {
    case EConsumableEffectType::BuffAttack:
        BuffBonusAttack += Amount;
        RecalculateTotalAttack();
        break;
    case EConsumableEffectType::BuffDefense:
        BuffBonusDefense += Amount;
        RecalculateTotalDefense();
        break;
    case EConsumableEffectType::BuffMaxHealth:
        BuffBonusMaxHealth += Amount;
        RecalculateTotalMaxHealth();
        break;
    case EConsumableEffectType::BuffMaxStamina:
        BuffBonusMaxStamina += Amount;
        RecalculateTotalMaxStamina();
        break;
    case EConsumableEffectType::BuffHealthGenRate:
        BuffBonusHealthGenRate += Amount;
        RecalculateTotalHealthGenRate();
        break;
    case EConsumableEffectType::BuffStaminaGenRate:
        BuffBonusStaminaGenRate += Amount;
        RecalculateTotalStaminaGenRate();
        break;
    case EConsumableEffectType::BuffWalkSpeed:
        BuffBonusWalkSpeed += Amount;
        RecalculateTotalWalkSpeed();
        break;
    case EConsumableEffectType::BuffMaxWeight:
        BuffBonusMaxWeight += Amount;
        RecalculateTotalMaxWeight();
        break;
    case EConsumableEffectType::BuffAttackSpeed:
        BuffBonusAttackSpeed += Amount;
        RecalculateTotalAttackSpeed();
        break;
    default:
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UCharacterStatComponent::ApplyStatBuff - Unhandled effect type: %s"), *UEnum::GetValueAsString(EffectType));
#endif
        return; // 처리되지 않는 효과는 타이머 설정 안 함
    }

#if WITH_EDITOR
    UE_LOG(LogTemp, Log, TEXT("Buff Applied: %s +%f for %f seconds."), *UEnum::GetValueAsString(EffectType), Amount, Duration);
#endif

    // 타이머 설정 (Duration이 0보다 커야 함)
    if (Duration > 0.0f)
    {
        FTimerHandle NewTimerHandle;
        FTimerDelegate TimerDelegate;
        // UFunction 바인딩 시 Enum과 Amount를 인자로 전달
        TimerDelegate.BindUFunction(this, FName("RemoveStatBuff"), EffectType, Amount);
        GetWorld()->GetTimerManager().SetTimer(NewTimerHandle, TimerDelegate, Duration, false);
        BuffTimerHandlesMap.Add(EffectType, NewTimerHandle);
    }

    // 모든 총합 스탯이 변경될 수 있으므로 다시 알림
    OnTotalStatsRecalculated.Broadcast();
}

void UCharacterStatComponent::SetAttrBonus(EAttrType Type, float Bonus)
{
    switch (Type)
    {
    case EAttrType::MaxStamina:
        AttrBonusMaxStamina = Bonus;
        break;
    case EAttrType::MaxHealth:
        AttrBonusMaxHealth = Bonus;
        break;
    case EAttrType::Defense:
        AttrBonusDefense = Bonus;
        break;
    case EAttrType::StaminaRegen:
        AttrBonusStaminaRegen = Bonus;
        break;
    case EAttrType::HealthRegen:
        AttrBonusHealthRegen = Bonus;
        break;
    case EAttrType::MaxCarryWeight:
        AttrBonusMaxCarryWeight = Bonus;
        break;
    case EAttrType::MoveSpeed:
        AttrBonusMoveSpeed = Bonus;
        break;
    }

    // 전체 갱신 한번
    RecalculateAllTotalStats();

	// 속성 보너스가 변경되면 관련 총합 스탯을 다시 계산
    OnTotalStatsRecalculated.Broadcast();
}

void UCharacterStatComponent::RemoveStatBuff(EConsumableEffectType EffectType, float Amount)
{
    if (EffectType == EConsumableEffectType::HealHealth || EffectType == EConsumableEffectType::RestoreStamina)
    {
        return;
    }

    // 스탯에서 Amount 감소
    switch (EffectType)
    {
    case EConsumableEffectType::BuffAttack:
        BuffBonusAttack -= Amount;
        RecalculateTotalAttack();
        break;
    case EConsumableEffectType::BuffDefense:
        BuffBonusDefense -= Amount;
        RecalculateTotalDefense();
        break;
    case EConsumableEffectType::BuffMaxHealth:
        BuffBonusMaxHealth -= Amount;
        RecalculateTotalMaxHealth();
        break;
    case EConsumableEffectType::BuffMaxStamina:
        BuffBonusMaxStamina -= Amount;
        RecalculateTotalMaxStamina();
        break;
    case EConsumableEffectType::BuffHealthGenRate:
        BuffBonusHealthGenRate -= Amount;
        RecalculateTotalHealthGenRate();
        break;
    case EConsumableEffectType::BuffStaminaGenRate:
        BuffBonusStaminaGenRate -= Amount;
        RecalculateTotalStaminaGenRate();
        break;
    case EConsumableEffectType::BuffWalkSpeed:
        BuffBonusWalkSpeed -= Amount;
        RecalculateTotalWalkSpeed();
        break;
    case EConsumableEffectType::BuffMaxWeight:
        BuffBonusMaxWeight -= Amount;
        RecalculateTotalMaxWeight();
        break;
    case EConsumableEffectType::BuffAttackSpeed:
        BuffBonusAttackSpeed -= Amount;
        RecalculateTotalAttackSpeed();
        break;
    default:
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UCharacterStatComponent::RemoveStatBuff - Unhandled effect type: %s"), *UEnum::GetValueAsString(EffectType));
#endif
        return;
    }

#if WITH_EDITOR
    UE_LOG(LogTemp, Log, TEXT("Buff Removed: %s -%f. Current BuffBonus%s: %f"), *UEnum::GetValueAsString(EffectType), Amount, *UEnum::GetValueAsString(EffectType), Amount);
#endif

    // 맵에서 타이머 핸들 제거
    BuffTimerHandlesMap.Remove(EffectType);

    // 모든 총합 스탯이 변경될 수 있으므로 다시 알림
    OnTotalStatsRecalculated.Broadcast();
}

void UCharacterStatComponent::ClearAllBuffTimers()
{
    for (auto& Elem : BuffTimerHandlesMap)
    {
        GetWorld()->GetTimerManager().ClearTimer(Elem.Value);
    }
    BuffTimerHandlesMap.Empty(); // 맵 비우기
}