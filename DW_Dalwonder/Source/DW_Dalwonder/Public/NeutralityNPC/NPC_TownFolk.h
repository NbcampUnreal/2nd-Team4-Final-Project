// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC_TownFolk.generated.h"

UCLASS()
class DW_DALWONDER_API ANPC_TownFolk : public ACharacter
{
	GENERATED_BODY()

public:
	ANPC_TownFolk();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="AI")
	TArray<AActor*> PatrolPoints;
};
