#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ConsumableSubData.generated.h"

USTRUCT(BlueprintType)
struct FConsumableSubData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Data")
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Data")
    float HealAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Data")
    float ManaRestoreAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Data")
    float Duration; 
};

