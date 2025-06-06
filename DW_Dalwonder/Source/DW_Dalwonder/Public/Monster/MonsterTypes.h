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
	TestNormal    UMETA(DisplayName = "TestNormal"),
	Dog1			UMETA(DisplayName = "Dog1"),
	StickMan		UMETA(DisplayName = "StickMan"),
	WoodMonster	UMETA(DisplayName = "WoodMonster"),
	Maynard	UMETA(DisplayName = "Maynard"),
	Drake	UMETA(DisplayName = "Drake"),
	SevarogRep UMETA(DisplayName = "SevarogRep"),
	SKnight_Warrior1 UMETA(DisplayName = "SKnight_Warrior1"),
	SKnight_Warrior2 UMETA(DisplayName = "SKnight_Warrior2"),
	SKnight_Warrior3 UMETA(DisplayName = "SKnight_Warrior3"),
	SKnight_Warrior4 UMETA(DisplayName = "SKnight_Warrior4"),
	SKnight_Archer UMETA(DisplayName = "SKnight_Archer"),
	SKnight_Sorcorer UMETA(DisplayName = "SKnight_Sorcorer"),
	SKnight_Suporter UMETA(DisplayName = "SKnight_Suporter"),
	Orc0 UMETA(DisplayName = "Orc0"),
	Orc1 UMETA(DisplayName = "Orc1"),
	Orc2 UMETA(DisplayName = "Orc2"),
	Cow UMETA(DisplayName = "Cow"),
	Pig UMETA(DisplayName = "Pig"),
	Chick UMETA(DisplayName = "Chick"),
};