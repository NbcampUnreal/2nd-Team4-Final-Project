#include "NeutralityNPC/ItemEnhanceManager.h"
#include "NeutralityNPC/ItemEnhanceTable.h"
#include "Inventory/InventoryComponent.h"
#include "Item/ItemBase.h"

UItemEnhanceManager::UItemEnhanceManager()
	: EnhanceDataTable(nullptr)
{
}

bool UItemEnhanceManager::TryEnhanceItem(UItemBase* TargetItem, UInventoryComponent* Inventory)
{
	check(EnhanceDataTable);

	// 데이터 테이블에서 강화 재료 확인
	FName ItemName = FName(TargetItem->ItemCode);
	FEnhanceItemTable* EnhanceTable = EnhanceDataTable->FindRow<FEnhanceItemTable>(ItemName, "");
	if (!EnhanceTable)
	{
		return false;
	}

	// 인벤토리에 충분한 양의 재료가 있는지 확인 후 소진
	for (FEnhanceItemData& Ingredient : EnhanceTable->IngredientItems)
	{
		int32 ItemIndex = Inventory->FindItemSlotIndex(Ingredient.ItemCode);
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

	// 확률에 따른 강화 성공
	float RandomFloat = FMath::FRand();
	float StarcatcherChance = 0.f;	// 스타캐처 보너스 확률
	if (RandomFloat + StarcatcherChance > EnhanceTable->EnhanceProbability)
	{
		return false;
	}

	// 제작 성공 시 아이템 지급
	int32 ItemQuantity = 1;
	int32 ItemIndex = Inventory->FindItemSlotIndex(TargetItem->ItemCode);
	Inventory->DropItemInSlot(ItemIndex, 1);
	TargetItem->ItemCode = GetEnhanceItemCode(TargetItem);
	TargetItem->LoadItemFromCode(TargetItem->ItemCode);
	Inventory->AddItem(TargetItem, ItemQuantity);
	return true;
}

FString UItemEnhanceManager::GetEnhanceItemCode(UItemBase* TargetItem)
{
	FString TargetCode = TargetItem->ItemCode;
	int32 CodeInt = FCString::Atoi(*TargetCode);
	
	CodeInt += 10;
	TargetCode = FString::FromInt(CodeInt);

	while (TargetCode.Len() < 4)
	{
		TargetCode = TEXT("0") + TargetCode;
	}

	return TargetCode;
}
