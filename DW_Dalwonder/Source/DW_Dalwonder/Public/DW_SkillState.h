#pragma once

#include "CoreMinimal.h"
#include "DW_SkillState.generated.h"

USTRUCT(BlueprintType)
struct FSkillState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel = 0;
};