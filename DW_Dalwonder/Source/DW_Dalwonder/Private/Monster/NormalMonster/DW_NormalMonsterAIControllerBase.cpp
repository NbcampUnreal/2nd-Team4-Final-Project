// Fill out your copyright notice in the Description page of Project Settings.



#include "Monster/NormalMonster/DW_NormalMonsterAIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/NormalMonster/DW_NormalMonsterBaseInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

const FName ADW_NormalMonsterAIControllerBase::CurrentStateKey = "CurrentState";
const FName ADW_NormalMonsterAIControllerBase::LastSeenLocationKey = "LastSeenLocation";

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
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1500.0f;
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

void ADW_NormalMonsterAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

};

void ADW_NormalMonsterAIControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!IsValid(Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("유효한 액터 아님"));
		return;
	}

	if (Actor->ActorHasTag("Player"))
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			bool bCanSeePlayer = Stimulus.WasSuccessfullySensed();
			BB = GetBlackboardComponent();
			if (BB)
			{
				BB->SetValueAsBool("bIsPlayerFound", bCanSeePlayer);

				if (bCanSeePlayer)
				{
					APawn* ControlledPawn = GetPawn();
					if (ControlledPawn && ControlledPawn->Implements<UDW_NormalMonsterBaseInterface>())
					{
						GetWorldTimerManager().ClearTimer(DelayLoseSightTimer);
						GetWorldTimerManager().ClearTimer(ChaseStopTimer);

						IDW_NormalMonsterBaseInterface::Execute_FoundPlayer(ControlledPawn);

						GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("See Player"));
						BB->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);
						StartChasingPlayer();
					}
				}
				else
				{
					BB->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);
					GetWorldTimerManager().SetTimer(DelayLoseSightTimer, this, &ADW_NormalMonsterAIControllerBase::HandleLoseSight, 5.f, false);

				}
			}
		}
		else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			ENormalMobState CurrentState = (ENormalMobState)BB->GetValueAsEnum(CurrentStateKey);
			if (CurrentState != ENormalMobState::Chasing)
			{
				if (BB)
				{
					BB->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);
					BB->SetValueAsEnum(CurrentStateKey, (uint8)ENormalMobState::Investigating);
				}
			}
		}
	}
}

void ADW_NormalMonsterAIControllerBase::HandleLoseSight()
{
	ENormalMobState CurrentState = (ENormalMobState)BB->GetValueAsEnum(CurrentStateKey);
	if (CurrentState == ENormalMobState::Chasing)
	{
		BB = GetBlackboardComponent();
		if (BB)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("Lose Player"));
			BB->SetValueAsEnum(CurrentStateKey, (uint8)ENormalMobState::Investigating);
		}

		GetWorldTimerManager().SetTimer(ChaseStopTimer, this, &ADW_NormalMonsterAIControllerBase::StopChasingPlayer, 10.f, false);

	}
}

void ADW_NormalMonsterAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}

	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsEnum(CurrentStateKey, (uint8)ENormalMobState::Idle);
	}
}

void ADW_NormalMonsterAIControllerBase::StartChasingPlayer()
{
	BB = GetBlackboardComponent();
	if (BB)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("Chase Player"));
		BB->SetValueAsEnum(CurrentStateKey, (uint8)ENormalMobState::Chasing);
	}
}

void ADW_NormalMonsterAIControllerBase::StopChasingPlayer()
{
	BB = GetBlackboardComponent();
	if (BB)
	{
		BB->SetValueAsEnum(CurrentStateKey, (uint8)ENormalMobState::Recall);
	}
}
