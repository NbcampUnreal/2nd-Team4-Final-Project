#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemEnhanceManager.generated.h"

class UInventoryComponent;
class UItemBase;

UCLASS()
class DW_DALWONDER_API UItemEnhanceManager : public UObject
{
	GENERATED_BODY()

public:
	UItemEnhanceManager();

	bool TryEnhanceItem(UItemBase* TargetItem, UInventoryComponent* Inventory);

	FString GetEnhanceItemCode(UItemBase* TargetItem);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
	UDataTable* EnhanceDataTable;
};
