#include "NeutralityNPC/NPC_Controller.h"

#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NPC/DW_NPC_TownFolk.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

ANPC_Controller::ANPC_Controller(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit.SetDefaultSubobjectClass<UCrowdFollowingComponent>(
			TEXT("PathFollowingComponent")))
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
	if (!MyNPCChar || !MyNPCChar->PatrolAnchorPoint) return;

	const FVector AnchorLocation = MyNPCChar->PatrolAnchorPoint->GetActorLocation();
	UNavigationSystemV1* NavSys  = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys) return;

	const float CapsuleR          = MyNPCChar->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const float AcceptableRadius  = CapsuleR + 120.f;          // 충돌 없는 도착 판정
	const float PersonalSpace     = 200.f;                     // ★ 개인 공간 2 m
	const float MinDistSq         = FMath::Square(PersonalSpace);

	FNavLocation RandomLocation;

	for (int32 Try = 0; Try < 5; ++Try)                        // ★ 최대 5회 샘플
	{
		if (NavSys->GetRandomReachablePointInRadius(
				AnchorLocation,
				MyNPCChar->RoamingRadius,
				RandomLocation) &&
			!IsTooCloseToOthers(RandomLocation.Location, MinDistSq))
		{
			// 조건 만족: 충분히 떨어진 지점 확보
			bIsMoving = true;
			MoveToLocation(RandomLocation.Location, AcceptableRadius);
			return;
		}
	}

	// 실패했으면 다음 틱에 다시 시도
	GetWorld()->GetTimerManager().SetTimerForNextTick(
		this, &ANPC_Controller::TryMoveToPatrol);
}

bool ANPC_Controller::IsTooCloseToOthers(const FVector& TestPos, float MinDistSq) const
{
	// 월드에 존재하는 모든 TownFolk 검사
	for (TActorIterator<ADW_NPC_TownFolk> It(GetWorld()); It; ++It)
	{
		const ADW_NPC_TownFolk* Other = *It;
		if (Other == GetPawn()) continue;                    // 자기 자신 제외

		if (FVector::DistSquared(Other->GetActorLocation(), TestPos) < MinDistSq)
		{
			return true;    // 너무 가까움
		}
	}
	return false;           // 충분히 멂
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
