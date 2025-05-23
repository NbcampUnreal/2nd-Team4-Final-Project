// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DW_NormalMonsterAIControllerBase.generated.h"

UENUM(BlueprintType)
enum class ENormalMobState : uint8
{
	Idle = 0,
	Investigating = 1,
	Chasing = 2,
	Recall = 3
};

class UAISenseConfig_Damage;
class UAISenseConfig_Hearing;
class UAISenseConfig_Sight;
struct FAIStimulus;

UCLASS()
class DW_DALWONDER_API ADW_NormalMonsterAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADW_NormalMonsterAIControllerBase();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Damage* DamageConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

public:
	UFUNCTION()
	void StartChasingPlayer();

	UFUNCTION()
	void StopChasingPlayer();

private:
	void HandleLoseSight();

	UBlackboardComponent* BB = nullptr;

	static const FName CurrentStateKey;
	static const FName LastSeenLocationKey;

	FTimerHandle ChaseStopTimer;
	FTimerHandle DelayLoseSightTimer;
};
