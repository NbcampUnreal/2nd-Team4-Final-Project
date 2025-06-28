#include "Character/CharacterArmorComponent.h"

#include "MediaSampleSink.h"
#include "Item/ItemBase.h"
#include "Character/DW_CharacterBase.h"
#include "Character/DW_Warrior.h"
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

void UCharacterArmorComponent::EquipArmor(UItemBase* Item)
{
	if (Item == nullptr) return;

	UDW_GameInstance* GameInstance = Cast<UDW_GameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance || !GameInstance->GetItemDataManager() || !GameInstance->GetItemDataManager()->ItemBaseDataTable)return;

	FItemData* ItemData = GameInstance->GetItemDataManager()->ItemBaseDataTable->FindRow<FItemData>(
		FName(*Item->ItemCode),
		TEXT("Lookup ItemCode")
	);

	if (!ItemData || ItemData->ItemType != EItemType::Equipment)
	{
		return;
	}

	ECharacterArmor ItemType = static_cast<ECharacterArmor>(ItemData->EquipSlot);

	if (ItemType == ECharacterArmor::Helmet)
	{
		Helmet = Item;
		Character->Helmet->SetSkeletalMeshAsset(GetArmorSkeletalMesh(Helmet));
	}
	else if (ItemType == ECharacterArmor::Armor)
	{
		Armor = Item;
		GetArmorSkeletalMesh(Armor);
	}
	else if (ItemType == ECharacterArmor::Glove)
	{
		Glove = Item;
		GetArmorSkeletalMesh(Glove);
	}
	else if (ItemType == ECharacterArmor::Boots)
	{
		Boots = Item;
		GetArmorSkeletalMesh(Boots);
	}
	else if (ItemType == ECharacterArmor::Weapon)
	{
		Weapon = Item;
		Character->SetWeapon(GetWeaponActor(Item));

		// 무기 타입 결정
		int32 WeaponType = FCString::Atoi(*Item->ItemCode) / 10000;
		if (ADW_Warrior* Warrior = Cast<ADW_Warrior>(Character))
		{
			Warrior->SetWeaponType(WeaponType);
		}
	}

	Character->UpdateSkeletalMesh();
}

USkeletalMesh* UCharacterArmorComponent::GetArmorSkeletalMesh(UItemBase* Item) const
{
	check(IsValid(ItemTable));

	//@TODO : ItemTable 에서 SkeletalMesh 가져오는 로직 작성
	FName RowName(FString::FromInt(FCString::Atoi(*Item->ItemCode) / 10000));
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

	//@TODO : ItemTable 에서 무기 액터 가져오는 로직 작성
	FName RowName(FString::FromInt(FCString::Atoi(*Item->ItemCode) / 10000));
	const FString ContextString(TEXT("ItemTable	Lookup"));
	FItemData* ItemDataRow = ItemTable->FindRow<FItemData>(RowName, ContextString);
	if (ItemDataRow == nullptr)
	{
		return nullptr;
	}

	return nullptr; // StaticCast<AActor*>(ItemDataRow->ItemMesh);
}
