#include "Character/CharacterArmorComponent.h"
#include "Item/ItemBase.h"
#include "Character/DW_CharacterBase.h"
#include "DW_GameInstance.h"
#include "Item/ItemDataManager.h"
#include "Item/ItemData.h"
#include "Engine/DataTable.h"

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
	if (Item == nullptr) return false;

	UDW_GameInstance* GameInstance = Cast<UDW_GameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance || !GameInstance->GetItemDataManager() || !GameInstance->GetItemDataManager()->ItemBaseDataTable)
	{
		return false;
	}
	
	FItemData* ItemData = GameInstance->GetItemDataManager()->ItemBaseDataTable->FindRow<FItemData>(
		FName(*Item->ItemCode),
		TEXT("Lookup ItemCode")
	);

	if (!ItemData || ItemData->ItemType != EItemType::Equipment)
	{
		return false;
	}

	EEquipSlotType ItemType = ItemData->EquipSlot;

	if (ItemType == EEquipSlotType::Helmet)
	{
		Helmet = Item;
		Character->Helmet->SetSkeletalMeshAsset(GetItemSkeletalMesh(Helmet));
	}
	else if (ItemType == EEquipSlotType::Chest)
	{
		Armor = Item;
		Character->Helmet->SetSkeletalMeshAsset(GetItemSkeletalMesh(Armor));
	}
	else if (ItemType == EEquipSlotType::Legs)
	{
		Pants = Item;
		Character->Helmet->SetSkeletalMeshAsset(GetItemSkeletalMesh(Pants));
	}
	else if (ItemType == EEquipSlotType::Gloves)
	{
		Glove = Item;
		Character->Helmet->SetSkeletalMeshAsset(GetItemSkeletalMesh(Glove));
	}
	else if (ItemType == EEquipSlotType::Boots)
	{
		Boots = Item;
		Character->Helmet->SetSkeletalMeshAsset(GetItemSkeletalMesh(Boots));
	}
	else if (ItemType == EEquipSlotType::Weapon)
	{
		Weapon = Item;
		Character->SetWeaponMesh(GetItemStaticMesh(Item));

		// 무기 타입 결정
		int32 WeaponCode = FCString::Atoi(*Item->ItemCode);
		if ((WeaponCode >= 1 && WeaponCode <= 11) || WeaponCode == 23 || WeaponCode == 24)
		{
			Character->SetWeaponType(1);
		}
		else if ((WeaponCode >= 12 && WeaponCode <= 22) || WeaponCode == 25 || WeaponCode == 26)
		{
			Character->SetWeaponType(0);
		}
	}

	Character->UpdateSkeletalMesh();
	return true;
}

UStaticMesh* UCharacterArmorComponent::GetItemStaticMesh(UItemBase* Item) const
{
	FString ItemCodeStr = Item->ItemCode;
	FItemData ItemData = ItemDataManager->GetItemDataFromCode(ItemCodeStr);
	return ItemData.ItemMesh.Get();
}

USkeletalMesh* UCharacterArmorComponent::GetItemSkeletalMesh(UItemBase* Item) const
{
	FString ItemCodeStr = Item->ItemCode;
	FItemData ItemData = ItemDataManager->GetItemDataFromCode(ItemCodeStr);
	return ItemData.ItemSkMesh.Get();
}
