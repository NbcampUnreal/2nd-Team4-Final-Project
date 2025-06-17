#include "Character/CharacterArmorComponent.h"

#include "MediaSampleSink.h"
#include "Item/ItemBase.h"
#include "Character/DW_CharacterBase.h"
#include "Character/DW_Warrior.h"

UCharacterArmorComponent::UCharacterArmorComponent()
	: Helmet(nullptr), Armor(nullptr), Glove(nullptr), Boots(nullptr), Weapon(nullptr)
{
	Character = Cast<ADW_CharacterBase>(GetOwner());
}

void UCharacterArmorComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UCharacterArmorComponent::EquipArmor(UItemBase* Item)
{
	ECharacterArmor ItemType = StaticCast<ECharacterArmor>(Item->ItemCode);

	if (ItemType == ECharacterArmor::Helmet)
	{
		Helmet = Item;
		GetArmorSkeletalMesh(Helmet);
		return true;
	}
	if (ItemType == ECharacterArmor::Armor)
	{
		Armor = Item;
		GetArmorSkeletalMesh(Armor);
		return true;
	}
	if (ItemType == ECharacterArmor::Glove)
	{
		Glove = Item;
		GetArmorSkeletalMesh(Glove);
		return true;
	}
	if (ItemType == ECharacterArmor::Boots)
	{
		Boots = Item;
		GetArmorSkeletalMesh(Boots);
		return true;
	}
	if (ItemType == ECharacterArmor::Weapon)
	{
		Weapon = Item;
		Character->SetWeapon(GetWeaponActor(Item));
		
		int32 WeaponType = Item->ItemCode / 10000;
		if (ADW_Warrior* Warrior = Cast<ADW_Warrior>(Character))
		{
			Warrior->SetWeaponType(WeaponType);
		}
		return true;
	}

	return false;
}

USkeletalMesh* UCharacterArmorComponent::GetArmorSkeletalMesh(UItemBase* Item) const
{
	check(IsValid(ItemTable));

	FName RowName(FString::FromInt(Item->ItemCode / 10000));
	const FString ContextString(TEXT("ItemTable	Lookup"));
	FItemData* ItemDataRow = ItemTable->FindRow<FItemData>(RowName, ContextString);
	if (ItemDataRow == nullptr)
	{
		return nullptr;
	}

	return nullptr; // StaticCast<USkeletalMesh*>(ItemDataRow->ItemMesh);
}

AActor* UCharacterArmorComponent::GetWeaponActor(UItemBase* Item) const
{
	check(IsValid(ItemTable));

	FName RowName(FString::FromInt(Item->ItemCode / 10000));
	const FString ContextString(TEXT("ItemTable	Lookup"));
	FItemData* ItemDataRow = ItemTable->FindRow<FItemData>(RowName, ContextString);
	if (ItemDataRow == nullptr)
	{
		return nullptr;
	}

	return nullptr; // StaticCast<AActor*>(ItemDataRow->ItemMesh);
}
