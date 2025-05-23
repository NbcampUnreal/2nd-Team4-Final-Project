#pragma once

#include "CoreMinimal.h"
#include "DW_SkillState.generated.h"

USTRUCT(BlueprintType)
struct FSkillState
{
    GENERATED_BODY()

    // 스킬 고유 ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SkillID;

    // 현재 스킬 레벨
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel = 0;
};