// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_NpcBase.h"
#include "DW_NPC_TownFolk.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API ADW_NPC_TownFolk : public ADW_NpcBase
{
	GENERATED_BODY()
	
public:
	ADW_NPC_TownFolk();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="AI")
	TArray<AActor*> PatrolPoints;
};
