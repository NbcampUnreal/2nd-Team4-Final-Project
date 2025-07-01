#include "Item/EquippableItem.h"
#include "Engine/Engine.h"           
#include "Components/SkeletalMeshComponent.h"
#include "Character/CharacterStatComponent.h"
#include "Character/CharacterArmorComponent.h"

UEquippableItem::UEquippableItem()
{
	ItemBaseData.ItemType = EItemType::Equipment;
}

bool UEquippableItem::EquipItem(AActor* Instigator)
{
	if (!Instigator)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("UEquippableItem::EquipItem - Instigator is null."));
#endif
		return false;
	}

	UCharacterStatComponent* CharacterStats = Instigator->FindComponentByClass<UCharacterStatComponent>();
	if (!CharacterStats)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("UEquippableItem::EquipItem - CharacterStatComponent not found on Instigator."));
#endif
		return false;
	}

	// 장비 스탯을 캐릭터 스탯 컴포넌트에 추가
	const FEquipmentStats& Stats = ItemBaseData.EquipmentStats;

	CharacterStats->AddEquipmentBonusMaxHealth(Stats.MaxHealthBonus);
	CharacterStats->AddEquipmentBonusHealthGenRate(Stats.HealthGenRateBonus);
	CharacterStats->AddEquipmentBonusMaxStamina(Stats.MaxStaminaBonus);
	CharacterStats->AddEquipmentBonusStaminaGenRate(Stats.StaminaGenRateBonus);
	CharacterStats->AddEquipmentBonusAttack(Stats.AttackBonus);
	CharacterStats->AddEquipmentBonusDefense(Stats.DefenseBonus);
	CharacterStats->AddEquipmentBonusMaxWeight(Stats.MaxWeightBonus);
	CharacterStats->AddEquipmentBonusAttackSpeed(Stats.AttackSpeedBonus);
	CharacterStats->AddEquipmentBonusWalkSpeed(Stats.WalkSpeedBonus);

	// 모든 총합 스탯을 다시 계산하도록 요청
	CharacterStats->RecalculateAllTotalStats();

#if WITH_EDITOR
	UE_LOG(LogTemp, Log, TEXT("Equipped Item: %s. Stats Applied: Health +%f, Attack +%f"),
		*ItemBaseData.ItemName.ToString(), Stats.MaxHealthBonus, Stats.AttackBonus);
#endif

	// 기본 ItemBase의 EquipItem 호출 (필요하다면)
	// Super::EquipItem(Instigator);

	UCharacterArmorComponent* CharacterArmors = Instigator->FindComponentByClass<UCharacterArmorComponent>();
	if (!CharacterArmors)
	{
		return false;
	}
	
	return CharacterArmors->EquipArmor(this); // 아이템 장착 성공
}

bool UEquippableItem::UnequipItem(AActor* Instigator)
{
	if (!Instigator)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("UEquippableItem::UnequipItem - Instigator is null."));
#endif
		return false;
	}

	UCharacterStatComponent* CharacterStats = Instigator->FindComponentByClass<UCharacterStatComponent>();
	if (!CharacterStats)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("UEquippableItem::UnequipItem - CharacterStatComponent not found on Instigator."));
#endif
		return false;
	}

	// 장비 스탯을 캐릭터 스탯 컴포넌트에서 제거 (Add 함수에 음수 값을 전달)
	const FEquipmentStats& Stats = ItemBaseData.EquipmentStats;

	CharacterStats->AddEquipmentBonusMaxHealth(-Stats.MaxHealthBonus);
	CharacterStats->AddEquipmentBonusHealthGenRate(-Stats.HealthGenRateBonus);
	CharacterStats->AddEquipmentBonusMaxStamina(-Stats.MaxStaminaBonus);
	CharacterStats->AddEquipmentBonusStaminaGenRate(-Stats.StaminaGenRateBonus);
	CharacterStats->AddEquipmentBonusAttack(-Stats.AttackBonus);
	CharacterStats->AddEquipmentBonusDefense(-Stats.DefenseBonus);
	CharacterStats->AddEquipmentBonusMaxWeight(-Stats.MaxWeightBonus);
	CharacterStats->AddEquipmentBonusAttackSpeed(-Stats.AttackSpeedBonus);
	CharacterStats->AddEquipmentBonusWalkSpeed(-Stats.WalkSpeedBonus);

	// 모든 총합 스탯을 다시 계산하도록 요청
	CharacterStats->RecalculateAllTotalStats();

#if WITH_EDITOR
	UE_LOG(LogTemp, Log, TEXT("Unequipped Item: %s. Stats Removed: Health -%f, Attack -%f"),
		*ItemBaseData.ItemName.ToString(), Stats.MaxHealthBonus, Stats.AttackBonus);
#endif
	// Super::UnequipItem(Instigator); 

	return true; // 아이템 해제 성공
}
