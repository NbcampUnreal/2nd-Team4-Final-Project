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

protected:
	// 현재 순찰 지점 인덱스
	int32 CurrentPatrolIndex = 0;
		
	// 현재 순찰 지점으로 이동하는 공통 함수
	void MoveToCurrentPatrolPoint();
};
