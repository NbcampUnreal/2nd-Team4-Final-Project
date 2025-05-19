// Fill out your copyright notice in the Description page of Project Settings.



#include "Monster/NormalMonster/DW_NormalMonsterAIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/NormalMonster/DW_NormalMonsterBaseInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"


// Sets default values
ADW_NormalMonsterAIControllerBase::ADW_NormalMonsterAIControllerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1800.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 100.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ADW_NormalMonsterAIControllerBase::OnTargetPerceptionUpdated);

}

void ADW_NormalMonsterAIControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!IsValid(Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("유효한 액터 아님"));
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		if (Actor->ActorHasTag("Player"))
		{
			APawn* ControlledPawn = GetPawn();
			if (ControlledPawn && ControlledPawn->Implements<UDW_NormalMonsterBaseInterface>())
			{
				IDW_NormalMonsterBaseInterface::Execute_FoundPlayer(ControlledPawn);
			}
		}
	}
	 else
	 {
	 	//GetBlackboardComponent()->ClearValue("TargetActor");
		GetBlackboardComponent()->SetValueAsBool("bIsPlayerFound", false);
	 }
}

void ADW_NormalMonsterAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}
