#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Item/ItemBase.h"
#include "ItemEnhanceTable.generated.h"

USTRUCT(BlueprintType)
struct FEnhanceItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};

USTRUCT(BlueprintType)
struct FEnhanceItemTable : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemCode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnhanceItemData> IngredientItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnhanceProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnhanceLevel;
};
