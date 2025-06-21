#include "NeutralityNPC/NPC_Controller.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NPC/DW_NPC_TownFolk.h"

void ANPC_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		NavSys->RegisterNavigationInvoker(this, 300.0f, 500.0f);
	}
	MoveToCurrentPatrolPoint();
}

void ANPC_Controller::MoveToCurrentPatrolPoint()
{
	ADW_NPC_TownFolk* MyNPCChar = Cast<ADW_NPC_TownFolk>(GetPawn());
	if (!MyNPCChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("MyNPCChar is null"));
		return;
	}

	if (MyNPCChar->PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PatrolPoints is empty"));
		return;
	}

	AActor* Target = MyNPCChar->PatrolPoints[CurrentPatrolIndex];
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Patrol Point is null"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Moving to Patrol Point: %s"), *Target->GetName());

	MoveToActor(Target, 5.0f, true, true, false, nullptr, true);

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
