#include "NeutralityNPC/NPC_Controller.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NPC/DW_NPC_TownFolk.h"

void ANPC_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	TryMoveToPatrol();
}

void ANPC_Controller::BeginPlay()
{
	Super::BeginPlay();
}

void ANPC_Controller::TryMoveToPatrol()
{
	if (GetPawn() == nullptr) return;

	FNavLocation Projected;
	if (UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(GetPawn()->GetActorLocation(), Projected))
	{
		// NavMesh 위에 있으므로 이동 시도
		MoveToCurrentPatrolPoint();
	}
	else
	{
		// NavMesh 아직 없음 → 다음 프레임에 재시도
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ANPC_Controller::TryMoveToPatrol);
	}
}

void ANPC_Controller::MoveToCurrentPatrolPoint()
{
	ADW_NPC_TownFolk* MyNPCChar = Cast<ADW_NPC_TownFolk>(GetPawn());
	if (!MyNPCChar)
		return;

	if (MyNPCChar->PatrolPoints.Num() == 0)
		return;

	AActor* Target = MyNPCChar->PatrolPoints[CurrentPatrolIndex];
	if (!Target)
		return;

	MoveToActor(Target, 5.0f, true, true, false, nullptr, true);
	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % MyNPCChar->PatrolPoints.Num();
}
	
void ANPC_Controller::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	if (Result.Code == EPathFollowingResult::Success)
	{
		TryMoveToPatrol();
	}
}
