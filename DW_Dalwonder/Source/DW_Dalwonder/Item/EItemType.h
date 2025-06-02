#pragma once
#include "CoreMinimal.h"
#include "EItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    None		   UMETA(DisplayName = "None"),
    Equipment      UMETA(DisplayName = "Equipment"),
    Consumable     UMETA(DisplayName = "Consumable"),
    Material       UMETA(DisplayName = "Material"),
    Quest          UMETA(DisplayName = "Quest")
};
