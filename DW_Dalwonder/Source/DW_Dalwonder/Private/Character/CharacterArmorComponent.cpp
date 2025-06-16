#include "Character/CharacterArmorComponent.h"
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
		AdjustArmorMesh(ECharacterArmor::Helmet);
		return true;
	}
	if (ItemType == ECharacterArmor::Armor)
	{
		Armor = Item;
		AdjustArmorMesh(ECharacterArmor::Armor);
		return true;
	}
	if (ItemType == ECharacterArmor::Glove)
	{
		Glove = Item;
		AdjustArmorMesh(ECharacterArmor::Glove);
		return true;
	}
	if (ItemType == ECharacterArmor::Boots)
	{
		Boots = Item;
		AdjustArmorMesh(ECharacterArmor::Boots);
		return true;
	}
	if (ItemType == ECharacterArmor::Weapon)
	{
		Weapon = Item;
		int32 WeaponType = Item->ItemCode / 10000;
		if (ADW_Warrior* Warrior = Cast<ADW_Warrior>(Character))
		{
			Warrior->SetWeaponType(WeaponType);
		}
		return true;
	}

	return false;
}

void UCharacterArmorComponent::AdjustArmorMesh(ECharacterArmor ArmorType)
{
	if (ArmorType == ECharacterArmor::Weapon)
	{
		AActor* WeaponActor = nullptr;// = Item->GetWeaponActor();
		if (IsValid(Character))
		{
			//Character->SetWeapon(WeaponActor);
		}
	}
	else
	{
		//USkeletalMesh* SkeletalMesh = Armor->GetArmorSkeletalMesh();
	}
}
