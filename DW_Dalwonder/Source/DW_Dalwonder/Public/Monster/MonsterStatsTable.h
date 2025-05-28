// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterStatsTable.generated.h"

USTRUCT(BlueprintType)
struct FMonsterStatsTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AccelSpeed;
	
};