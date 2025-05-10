// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BossMonster/DW_BossMonsterBase.h"
#include "DW_Sevarog.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Sevarog : public ADW_BossMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_Sevarog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
