#pragma once

#include "CoreMinimal.h"
#include "EItemGrade.generated.h"

UENUM(BlueprintType)
enum class EItemGrade : uint8
{
    // 이미지에 기반한 등급
    Normal       UMETA(DisplayName = "노멀"),      
    Rare         UMETA(DisplayName = "레어"),      
    Unique       UMETA(DisplayName = "유니크"),    
    Legendary    UMETA(DisplayName = "레전더리")   
};
