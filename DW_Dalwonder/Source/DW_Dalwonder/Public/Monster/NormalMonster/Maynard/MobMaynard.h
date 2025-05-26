// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "MobMaynard.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API AMobMaynard : public ADW_NormalMonsterBase
{
	GENERATED_BODY()
	
private:
	AMobMaynard();

public:
	UFUNCTION(BlueprintCallable)
	int32 GetMaynardType();

private:
	int32 MaynardType;
};
