#include "NeutralityNPC/PatrolPath.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;  // 매 프레임 갱신 불필요
}

AActor* APatrolPath::GetWaypoint(int32 Index) const
{
	if (Waypoints.IsValidIndex(Index))
	{
		return Waypoints[Index];
	}
	return nullptr;
}

int32 APatrolPath::Num() const
{
	return Waypoints.Num();
}

