// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/DW_BossMonsterAIControllerBase.h"


// Sets default values
ADW_BossMonsterAIControllerBase::ADW_BossMonsterAIControllerBase(): BehaviorTreeAsset(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ADW_BossMonsterAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}
