#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NeutralityNPC/ItemCraftTable.h"
#include "Item/EItemGrade.h"
#include "ItemCraftManager.generated.h"

class UInventoryComponent;
class UItemBase;

UCLASS()
class DW_DALWONDER_API UItemCraftManager : public UObject
{
	GENERATED_BODY()

public:
	UItemCraftManager();

	bool TryCraftItem(UItemBase* TargetItem, UInventoryComponent* Inventory, int32 Quantity);

	EItemGrade GetItemGrade(float BonusChance);

	FString GetUpgradeItemCode(UItemBase* TargetItem, EItemGrade ItemGrade);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
	UDataTable* CraftDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
	TMap<EItemGrade, float> ItemGradeProbability;
};
