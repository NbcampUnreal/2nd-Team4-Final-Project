#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Item/ItemBase.h"
#include "ItemCraftTable.generated.h"

USTRUCT(BlueprintType)
struct FCraftItemTable : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UItemBase*, int32> IngredientItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CraftProbability;
};
