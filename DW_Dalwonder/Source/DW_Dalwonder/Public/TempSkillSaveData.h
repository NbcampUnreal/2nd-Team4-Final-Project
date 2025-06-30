// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_SkillState.h"
#include "TempSkillSaveData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTempSkillSaveData
{
    GENERATED_BODY()

    // 사용 가능한 스킬 포인트
    UPROPERTY() int32 CurrentSP = 0;

    // 스킬 트리 상태 (활성화 여부, 레벨 등)
    UPROPERTY()
    TMap<FName, FSkillState> SkillStates;

    // 마스터리 및 성장 정보
    UPROPERTY() int32 LevelUpCount = 0;
    UPROPERTY() float CurrentMastery = 0.f;
    UPROPERTY() float MaxMastery = 0.f;
};
