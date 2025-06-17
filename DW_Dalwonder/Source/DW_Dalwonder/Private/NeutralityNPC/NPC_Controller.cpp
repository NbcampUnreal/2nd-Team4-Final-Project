#include "NeutralityNPC/NPC_Controller.h"
#include "NeutralityNPC/NPC_TownFolk.h"
#include "Navigation/PathFollowingComponent.h"

void ANPC_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ANPC_Controller::BeginPlay()
{
	Super::BeginPlay();
	MoveToCurrentPatrolPoint();
}

void ANPC_Controller::MoveToCurrentPatrolPoint()
{
	ANPC_TownFolk* MyNPCChar = Cast<ANPC_TownFolk>(GetPawn());
	if (!MyNPCChar)
	{
		return;
	}

	// 순찰 포인트가 하나도 없다면 이동할 필요 없음
	if (MyNPCChar->PatrolPoints.Num() == 0)
	{
		return;
	}
    
	MoveToActor(
		MyNPCChar->PatrolPoints[CurrentPatrolIndex],
		5.0f,   // AcceptanceRadius: 목표 지점 근처 몇 유닛 이내에 도달하면 멈출지
		true,   // bStopOnOverlap
		true,   // bUsePathfinding
		false,  // bCanStrafe: 기본 이동 모드에서 좌우로 회전 없이 이동 가능 여부
		nullptr,// FilterClass: 경로 필터. 디폴트 사용
		true    // bAllowPartialPath: 부분 경로 허용 여부
	);

	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % MyNPCChar->PatrolPoints.Num();
}
	
void ANPC_Controller::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	if (Result.Code == EPathFollowingResult::Success)
	{
		MoveToCurrentPatrolPoint();
	}
}
