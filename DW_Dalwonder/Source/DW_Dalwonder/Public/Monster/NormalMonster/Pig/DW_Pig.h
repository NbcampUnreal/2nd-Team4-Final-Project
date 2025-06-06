// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "DW_Pig.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Pig : public ADW_NormalMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_Pig();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
