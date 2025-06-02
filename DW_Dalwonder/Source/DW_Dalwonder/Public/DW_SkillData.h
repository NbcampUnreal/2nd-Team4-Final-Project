#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DW_SkillData.generated.h"

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Cost = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName PrerequisiteSkillID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Increase;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 IncreaseType; // 1: 퍼센트, 2: 절댓값

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* IconActivated;
};
