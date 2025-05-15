#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Equipment      UMETA(DisplayName = "Equipment"),
    Consumable     UMETA(DisplayName = "Consumable"),
    Material       UMETA(DisplayName = "Material"),
    Quest          UMETA(DisplayName = "Quest")
    
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxStackSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value;
};
