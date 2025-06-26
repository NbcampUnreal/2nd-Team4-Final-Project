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
    FName PrerequisiteSkillID = "None";

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Increase;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 IncreaseType; // 1: %, 2: 고정값

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* IconActivated;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CoolTime = 0.f; // 새로 추가된 열
};
