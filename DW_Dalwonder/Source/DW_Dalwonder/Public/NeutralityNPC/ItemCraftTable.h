#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemCraftTable.generated.h"

USTRUCT(BlueprintType)
struct FCraftItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemCode;

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
	FText ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCraftItemData> IngredientItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CraftProbability;
};
