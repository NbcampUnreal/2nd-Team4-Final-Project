// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DW_AttributeSaveData.h"
#include "UI/Component/Struct/QuestData.h"
#include "Inventory/InventoryComponent.h"
#include "InventorySlotSaveData.h"
#include "CharacterStatSaveData.h"
#include "QuestProgressSaveData.h"
#include "EquippedArmorSaveData.h"
#include "SkillSystemMetaData.h"
#include "BitmaskFogData.h"
#include "DW_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDW_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

    // 위치 정보
    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    FVector SavedPlayerLocation;

    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    FRotator SavedPlayerRotation;

    // 스탯 데이터 (CharacterStatComponent)
    UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FCharacterStatSaveData SavedCharacterStat;

    // 스킬 트리 상태 (SkillComponent)
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FSkillSystemSaveData SavedSkillData;

    // 진행 중인 퀘스트
    UPROPERTY()
    TArray<FQuestProgressSaveData> SaveAcceptedQuests;

    // 완료된 퀘스트 ID만 저장
    UPROPERTY()
    TArray<FName> SaveCompletedQuestIDs;

	//인벤토리(ItemCode와 수량 저장)
    UPROPERTY()
    TArray<FInventorySlotSaveData> TempInventorySlotSaveDatas;

	// 인벤토리 슬롯 수
    UPROPERTY()
    int32 SavedInventorySlotQuantity = 0;

    //방어구/무기 추가
    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    FEquippedArmorSaveData SavedArmorData;

	/** 맵별 Bitmask 저장된 안개 정보 */
	UPROPERTY()
	TMap<FName, FBitmaskFogData> SavedFogMap;

	// 감도
	UPROPERTY()
	float SavedMouseSensitivity = 1.0f;

	// UI 그래픽
	UPROPERTY(BlueprintReadWrite)
	int32 SavedWindowMode;

	UPROPERTY(BlueprintReadWrite)
	FIntPoint SavedResolution;

	UPROPERTY(BlueprintReadWrite)
	bool bSavedVSync;
	
	UPROPERTY()
	float SavedFrameRate;

	UPROPERTY()
	bool bSavedMotionBlur;

	UPROPERTY()
	bool bSavedShadow;
	
	// 사운드
	UPROPERTY()
	float SavedVolumeMaster = 100.f;

	UPROPERTY()
	float SavedVolumeBGM = 100.f;

	UPROPERTY()
	float SavedVolumeSFX = 100.f;

	UPROPERTY()
	float SavedVolumeUI = 100.f;
};
