#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Item/ItemBase.h"
#include "ItemCraftTable.generated.h"

USTRUCT(BlueprintType)
struct FCraftItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};

USTRUCT(BlueprintType)
struct FCraftItemTable : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemCode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCraftItemData> IngredientItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CraftProbability;
};
