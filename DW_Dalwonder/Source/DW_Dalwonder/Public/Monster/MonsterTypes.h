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
	Sevarog			UMETA(DisplayName = "Sevarog"),
	Stickman		UMETA(DisplayName = "Stickman"),
	WoodMonster		UMETA(DisplayName = "WoodMonster"),
};