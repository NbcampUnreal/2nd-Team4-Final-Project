// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjective.generated.h"

// 퀘스트 종류
UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
    Kill,
    Collect,
    Talk,
    Move,
    Custom
};

/**
 * 
 */

USTRUCT(BlueprintType)
struct FQuestObjective
{
    GENERATED_BODY()

    // 어떤 종류의 목표인지
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EObjectiveType ObjectiveType;

    // 대상의 ID (예: 적 타입 이름, 아이템 ID 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TargetID;

    // 목표 달성에 필요한 횟수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredCount = 1;

    // 현재까지 달성한 횟수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentCount = 0;

    // 이 목표가 완료되었는가?
    bool IsCompleted() const
    {
        return CurrentCount >= RequiredCount;
    }
};
