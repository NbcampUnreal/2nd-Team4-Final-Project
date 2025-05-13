// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "DW_Dog1.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Dog1 : public ADW_NormalMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_Dog1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
