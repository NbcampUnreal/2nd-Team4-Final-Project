// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_AttributeSaveData.h"
#include "DW_SkillState.h"
#include "UI/Component/Struct/QuestData.h"
#include "Inventory/InventoryComponent.h"
#include "TmpInventoryData.h"
#include "TmpSaveData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTmpSaveData
{
    GENERATED_BODY()

    // 위치 정보
    UPROPERTY()
    FVector PlayerLocation;

    UPROPERTY()
    FRotator PlayerRotation;

    // 속성 데이터 (AttributeComponent)
    UPROPERTY()
    FAttributeSaveData TempAttributes;

    // 스킬 트리 상태 (SkillComponent)
    UPROPERTY()
    TMap<FName, FSkillState> TempSkillStates;

    // 퀘스트 상태 (QuestManagerComponent)
    UPROPERTY()
    TMap<FName, FQuestData> TempAcceptedQuests;

    UPROPERTY()
    TMap<FName, FQuestData> TempCompletedQuests;


    //인벤토리
    UPROPERTY()
    FTmpInventoryData TempInventory;
    // 여기에 추가항목들
};
