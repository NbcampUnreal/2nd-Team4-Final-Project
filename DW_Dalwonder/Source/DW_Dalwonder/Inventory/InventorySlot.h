#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "InventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FInventoryItem Item;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOccupied = false;

    FInventorySlot() { bIsOccupied = false; }
};
