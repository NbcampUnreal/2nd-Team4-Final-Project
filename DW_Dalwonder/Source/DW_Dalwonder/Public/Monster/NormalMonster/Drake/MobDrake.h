// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "MobDrake.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API AMobDrake : public ADW_NormalMonsterBase
{
	GENERATED_BODY()
	
private:
	AMobDrake();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HeightLocation;
};
