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

void UCharacterArmorComponent::EquipArmor(UItemBase* Item)
{
	//@TODO : ItemBase 의 코드를 통해 현재 장착하려는 아이템이 어느 부위인지 구분짓기
	ECharacterArmor ItemType = StaticCast<ECharacterArmor>(Item->ItemCode);

	if (ItemType == ECharacterArmor::Helmet)
	{
		Helmet = Item;
		Character->Helmet->SetSkeletalMeshAsset(GetArmorSkeletalMesh(Helmet));
	}
	if (ItemType == ECharacterArmor::Armor)
	{
		Armor = Item;
		GetArmorSkeletalMesh(Armor);
	}
	if (ItemType == ECharacterArmor::Glove)
	{
		Glove = Item;
		GetArmorSkeletalMesh(Glove);
	}
	if (ItemType == ECharacterArmor::Boots)
	{
		Boots = Item;
		GetArmorSkeletalMesh(Boots);
	}
	if (ItemType == ECharacterArmor::Weapon)
	{
		Weapon = Item;
		Character->SetWeapon(GetWeaponActor(Item));

		//@TODO : 무기 종류를 ItemBase 를 통해 구분하여 캐릭터에게 현재 장착한 무기의 타입 지정해주기
		int32 WeaponType = Item->ItemCode / 10000;
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

	//@TODO : ItemTable 에서 무기 액터 가져오는 로직 작성
	FName RowName(FString::FromInt(Item->ItemCode / 10000));
	const FString ContextString(TEXT("ItemTable	Lookup"));
	FItemData* ItemDataRow = ItemTable->FindRow<FItemData>(RowName, ContextString);
	if (ItemDataRow == nullptr)
	{
		return nullptr;
	}

	return nullptr; // StaticCast<AActor*>(ItemDataRow->ItemMesh);
}
