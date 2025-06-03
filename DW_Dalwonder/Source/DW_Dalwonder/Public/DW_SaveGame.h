// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DW_AttributeSaveData.h"
#include "DW_SkillState.h"
#include "DW_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDW_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// 캐릭터 위치를 저장할 변수
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FVector SavedPlayerLocation;
	// 캐릭터 기본 스탯 저장 변수(스킬트리 값을 불러와서 올려줄꺼임)
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FAttributeSaveData SavedAttributes;

	// 스킬 트리 데이터 저장
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TMap<FName, FSkillState> SavedSkillStates;
};
