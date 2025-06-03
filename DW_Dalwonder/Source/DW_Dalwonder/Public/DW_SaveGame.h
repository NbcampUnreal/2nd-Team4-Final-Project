// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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

};
