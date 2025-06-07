#include "Character/CharacterArmorComponent.h"
#include "WeaponBase.h"

UCharacterArmorComponent::UCharacterArmorComponent()
	: Helmet(nullptr), Armor(nullptr), Glove(nullptr), Boots(nullptr), Weapon(nullptr)
{
	
}

void UCharacterArmorComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UCharacterArmorComponent::EquipArmor(AWeaponBase* Item)
{
	// 임시 함수
	ECharacterArmor WeaponType = ECharacterArmor::Glove;// = Item->WeaponID.ToString()[0]

	if (WeaponType == ECharacterArmor::Helmet)
	{
		AdjustArmorMesh(Item, ECharacterArmor::Helmet);
		return true;
	}
	if (WeaponType == ECharacterArmor::Armor)
	{
		AdjustArmorMesh(Item, ECharacterArmor::Armor);
		return true;
	}
	if (WeaponType == ECharacterArmor::Glove)
	{
		AdjustArmorMesh(Item, ECharacterArmor::Glove);
		return true;
	}
	if (WeaponType == ECharacterArmor::Boots)
	{
		AdjustArmorMesh(Item, ECharacterArmor::Boots);
		return true;
	}
	if (WeaponType == ECharacterArmor::Weapon)
	{
		AdjustArmorMesh(Item, ECharacterArmor::Weapon);
		return true;
	}

	return false;
}

void UCharacterArmorComponent::AdjustArmorMesh(AWeaponBase* Item, ECharacterArmor ArmorType)
{
	if (ArmorType == ECharacterArmor::Weapon)
	{
		USceneComponent* SceneRoot = Item->ItemActor->SceneRoot;
		UStaticMeshComponent* StaticMesh = Item->ItemActor->MeshComponent;
		Weapon = SceneRoot;
		StaticMesh->SetupAttachment(Weapon);
	}
	else
	{
		USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(Item->ItemActor->MeshComponent);
		if (ArmorType == ECharacterArmor::Helmet)
		{
			Helmet = SkeletalMesh->GetSkeletalMeshAsset();
		}
		else if (ArmorType == ECharacterArmor::Armor)
		{
			Armor = SkeletalMesh->GetSkeletalMeshAsset();
		}
		else if (ArmorType == ECharacterArmor::Glove)
		{
			Glove = SkeletalMesh->GetSkeletalMeshAsset();
		}
		else if (ArmorType == ECharacterArmor::Boots)
		{
			Boots = SkeletalMesh->GetSkeletalMeshAsset();
		}
	}
}
