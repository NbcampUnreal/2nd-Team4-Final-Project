#pragma once

#include "CoreMinimal.h"
#include "Item/itemData.h"
#include "InventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FItemData ItemData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOccupied = false;

    FInventorySlot() { bIsOccupied = false; }
};
