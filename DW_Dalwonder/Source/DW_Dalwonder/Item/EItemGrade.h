#pragma once

#include "CoreMinimal.h"
#include "EItemGrade.generated.h"

UENUM(BlueprintType)
enum class EItemGrade : uint8
{
	UnKnown      UMETA(DisplayName = "알 수 없음"),
    Normal       UMETA(DisplayName = "노멀"),      
    Rare         UMETA(DisplayName = "레어"),      
    Unique       UMETA(DisplayName = "유니크"),    
    Legendary    UMETA(DisplayName = "레전더리")   
};
