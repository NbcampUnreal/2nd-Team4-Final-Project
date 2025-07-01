#include "NeutralityNPC/ItemCraftManager.h"
#include "NeutralityNPC/ItemCraftTable.h"
#include "Inventory/InventoryComponent.h"
#include "Item/ItemBase.h"

UItemCraftManager::UItemCraftManager()
	: CraftDataTable(nullptr)
{
	ItemGradeProbability = {
	{EItemGrade::Normal, 60.f},
	{EItemGrade::Rare, 25.f},
	{EItemGrade::Unique, 14.f},
	{EItemGrade::Legendary, 1.f}
	};
}

bool UItemCraftManager::TryCraftItem(UItemBase* TargetItem, UInventoryComponent* Inventory, int32 Quantity)
{
	check(CraftDataTable);
	
	// 데이터 테이블에서 재료 레시피 확인
	FName ItemName = FName(TargetItem->ItemCode);
	FCraftItemTable* CraftRecipe = CraftDataTable->FindRow<FCraftItemTable>(ItemName, "");
	if (!CraftRecipe)
	{
		return false;
	}
	
	// 인벤토리에 충분한 양의 재료가 있는지 확인 후 소진
	for (FCraftItemData& Ingredient : CraftRecipe->IngredientItems)
	{
		int32 ItemIndex = Inventory->FindItemSlotIndex(Ingredient.ItemClass.Get());
		if (ItemIndex == -1)
		{
			return false;
		}
	
		if (Inventory->InventorySlots[ItemIndex].Quantity < Ingredient.Amount)
		{
			return false;
		}
	
		Inventory->DropItemInSlot(ItemIndex, Ingredient.Amount);
	}
	
	// 확률에 따른 제작 성공
	float RandomFloat = FMath::FRand();
	if (RandomFloat > CraftRecipe->CraftProbability)
	{
		return false;
	}
	
	// 제작 성공 시 아이템 지급
	float StarcatcherChance = 0.f;	// 스타캐처 보너스 확률
	EItemGrade ItemGrade = GetItemGrade(StarcatcherChance);
	TargetItem->ItemCode = GetUpgradeItemCode(TargetItem, ItemGrade);
	TargetItem->LoadItemFromCode(TargetItem->ItemCode);
	Inventory->AddItem(TargetItem, Quantity);
	return true;
}

EItemGrade UItemCraftManager::GetItemGrade(float BonusChance)
{
	float TotalChance = 0.f;
	for (auto Chance : ItemGradeProbability)
	{
		TotalChance += Chance.Value;
	}

	if (TotalChance <= 0.f)
	{
		return EItemGrade::UnKnown;
	}
	
	float RandomFloat = FMath::FRandRange(0.f, TotalChance);
	float CumulativeChance = 0.f;

	for (auto Chance : ItemGradeProbability)
	{
		CumulativeChance += Chance.Value;
		if (RandomFloat <= CumulativeChance + BonusChance)
		{
			return Chance.Key;
		}
	}

	return EItemGrade::UnKnown;
}

FString UItemCraftManager::GetUpgradeItemCode(UItemBase* TargetItem, EItemGrade ItemGrade)
{
	FString TargetCode = TargetItem->ItemCode;
	int32 CodeInt = FCString::Atoi(*TargetCode);

	if (ItemGrade == EItemGrade::Normal)
	{
		
	}
	if (ItemGrade == EItemGrade::Rare)
	{
		CodeInt += 100;
		TargetCode = FString::FromInt(CodeInt);
	}
	if (ItemGrade == EItemGrade::Unique)
	{
		CodeInt += 200;
		TargetCode = FString::FromInt(CodeInt);
	}
	if (ItemGrade == EItemGrade::Legendary)
	{
		CodeInt += 300;
		TargetCode = FString::FromInt(CodeInt);
	}

	if (TargetCode.Len() < 4)
	{
		TargetCode = TEXT("0") + TargetCode;
	}

	return TargetCode;
}
