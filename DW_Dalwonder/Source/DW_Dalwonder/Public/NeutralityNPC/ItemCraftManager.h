#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NeutralityNPC/ItemCraftTable.h"
#include "ItemCraftManager.generated.h"

class UInventoryComponent;
class UItemBase;
struct FInventorySlot;

UCLASS()
class DW_DALWONDER_API UItemCraftManager : public UObject
{
	GENERATED_BODY()

public:
	UItemCraftManager();

	bool TryCraftItem(UItemBase* TargetItem, UInventoryComponent* Inventory, int32 Quantity);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
	UDataTable* CraftDataTable;
};
