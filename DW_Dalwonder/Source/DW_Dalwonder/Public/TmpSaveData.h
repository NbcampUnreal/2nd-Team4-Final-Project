// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_AttributeSaveData.h"
#include "TempSkillSaveData.h"
#include "UI/Component/Struct/QuestData.h"
#include "Inventory/InventoryComponent.h"
#include "TmpInventoryData.h"
#include "TmpArmorData.h"
#include "TmpCharacterStatData.h"
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

	// 스탯 데이터 (CharacterStatComponent)
    UPROPERTY()
    FTmpCharacterStatData TempStatData;

    // 스킬 상태 (SkillComponent)
    UPROPERTY()
    FTempSkillSaveData TempSkillData;

    // 퀘스트 상태 (QuestManagerComponent)
    UPROPERTY()
    TMap<FName, FQuestData> TempAcceptedQuests;

    UPROPERTY()
    TMap<FName, FQuestData> TempCompletedQuests;

    //인벤토리
    UPROPERTY()
    FTmpInventoryData TempInventory;

    //방어구/무기 추가
    UPROPERTY()
    FTmpArmorData TempArmor;
    // 여기 아래에 추가항목들 계속 추가가능...
};
