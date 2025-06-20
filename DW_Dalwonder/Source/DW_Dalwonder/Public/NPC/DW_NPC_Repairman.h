// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_NpcBase.h"
#include "DW_NPC_Repairman.generated.h"

class UAnimMontage;

UCLASS()
class DW_DALWONDER_API ADW_NPC_Repairman : public ADW_NpcBase
{
	GENERATED_BODY()
public:
	void BeginPlay();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Repairman|Animation")
	UAnimMontage* EnhanceMontage;

	void PlayEnhanceAnimation();
};
