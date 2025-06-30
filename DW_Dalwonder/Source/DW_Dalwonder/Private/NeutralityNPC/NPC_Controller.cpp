#include "NeutralityNPC/NPC_Controller.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NPC/DW_NPC_TownFolk.h"
#include "TimerManager.h"

ANPC_Controller::ANPC_Controller()
{
	bIsMoving = false;
}

void ANPC_Controller::BeginPlay()
{
	Super::BeginPlay();
}

void ANPC_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 시작 시 이동 시도
	TryMoveToPatrol();
}

void ANPC_Controller::TryMoveToPatrol()
{
	if (bIsMoving || GetPawn() == nullptr)
		return;

	FNavLocation Projected;
	if (UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(GetPawn()->GetActorLocation(), Projected))
	{
		MoveToCurrentPatrolPoint();
	}
	else
	{
		// NavMesh 없으면 다음 프레임에 다시 시도
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ANPC_Controller::TryMoveToPatrol);
	}
}

void ANPC_Controller::MoveToCurrentPatrolPoint()
{
	ADW_NPC_TownFolk* MyNPCChar = Cast<ADW_NPC_TownFolk>(GetPawn());
	if (!MyNPCChar || MyNPCChar->PatrolPoints.Num() == 0)
		return;

	AActor* Target = MyNPCChar->PatrolPoints[CurrentPatrolIndex];
	if (!Target)
		return;

	bIsMoving = true;

	MoveToActor(Target, 5.0f, true, true, false, nullptr, true);
	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % MyNPCChar->PatrolPoints.Num();
}

void ANPC_Controller::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	bIsMoving = false;

	if (Result.Code == EPathFollowingResult::Success)
	{
		// 딜레이 후 다음 포인트로 이동
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ANPC_Controller::TryMoveToPatrol);
	}
}
