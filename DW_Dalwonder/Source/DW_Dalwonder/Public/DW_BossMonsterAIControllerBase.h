// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DW_BossMonsterAIControllerBase.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_BossMonsterAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADW_BossMonsterAIControllerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;
};
