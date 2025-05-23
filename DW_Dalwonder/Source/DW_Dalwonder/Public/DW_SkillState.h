#pragma once

#include "CoreMinimal.h"
#include "DW_SkillState.generated.h"

USTRUCT(BlueprintType)
struct FSkillState
{
    GENERATED_BODY()

    // ��ų ���� ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SkillID;

    // ���� ��ų ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel = 0;
};