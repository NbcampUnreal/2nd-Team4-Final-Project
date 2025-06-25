#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPC_Controller.generated.h"

UCLASS()
class DW_DALWONDER_API ANPC_Controller : public AAIController
{
	GENERATED_BODY()
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	void BeginPlay();

protected:
	void TryMoveToPatrol();
	void MoveToCurrentPatrolPoint();

	UPROPERTY()
	TArray<AActor*> PatrolPoints;

	int32 CurrentPatrolIndex = 0;

	FTimerHandle PatrolRetryHandle;
};
