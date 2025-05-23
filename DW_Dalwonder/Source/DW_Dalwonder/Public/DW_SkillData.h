#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DW_SkillData.generated.h"

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
    GENERATED_BODY()

    // 스킬의 고유 ID
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName SkillID;

    // 스킬 이름 (UI에 표시)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText SkillName;

    // 스킬 설명
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;

    // 이 스킬을 찍기 위해 소모되는 SP
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Cost = 1;

    // 이 스킬을 배우기 위한 선행 스킬 ID
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName PrerequisiteSkillID;

    // 스킬 최대 레벨 (예: 1~5)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxLevel = 1;

    // 스킬 비활성화 상태일 때 아이콘
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Icon;

    // 스킬 활성화 상태일 때 아이콘
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* IconActivated;
};