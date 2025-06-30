#include "NeutralityNPC/ItemCraftManager.h"
#include "NeutralityNPC/ItemCraftTable.h"
#include "Inventory/InventoryComponent.h"


UItemCraftManager::UItemCraftManager()
	: CraftDataTable(nullptr)
{
	
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
		int32 ItemIndex = Inventory->FindItemSlotIndex(Ingredient.ItemClass);
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
	float BonusChance = 0.f;	// 스타캐처 보너스 확률
	float RandomFloat = FMath::FRand();
	if (RandomFloat >  CraftRecipe->CraftProbability + BonusChance)
	{
		return false;
	}

	// 제작 성공 시 아이템 지급
	Inventory->AddItem(TargetItem, Quantity);
	return true;
}