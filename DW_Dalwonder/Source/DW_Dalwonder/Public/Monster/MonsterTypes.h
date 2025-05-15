// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterTypes.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Spawn		UMETA(DisplayName = "Spawn"),
	Patrol      UMETA(DisplayName = "Patrol"),    
	Chase       UMETA(DisplayName = "Chase"),     
	Dead        UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class EMonsterName : uint8
{
	Sevarog        UMETA(DisplayName = "Sevarog"),
	SKnight		UMETA(DisplayName = "SKnight"),
	TestNormal    UMETA(DisplayName = "TestNormal"),
	Dog1			UMETA(DisplayName = "Dog1"),
	StickMan		UMETA(DisplayName = "StickMan"),
	WoodMonster	UMETA(DisplayName = "WoodMonster"),
};