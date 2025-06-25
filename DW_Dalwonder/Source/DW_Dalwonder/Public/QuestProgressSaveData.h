// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestProgressSaveData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FQuestProgressSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    FName QuestID;

    UPROPERTY()
    TArray<int32> ObjectiveProgress; // 각 Objective의 CurrentCount 저장
};
