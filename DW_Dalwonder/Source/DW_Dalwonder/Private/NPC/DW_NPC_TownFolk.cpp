#include "NPC/DW_NPC_TownFolk.h"

#include "NavigationInvokerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NeutralityNPC/NPC_Controller.h"

ADW_NPC_TownFolk::ADW_NPC_TownFolk()
{
	// ── AI 컨트롤러 & 네비게이터 ───────────────────
	AIControllerClass = ANPC_Controller::StaticClass();
	AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;

	NavInvokerComp = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));
	NavInvokerComp->SetGenerationRadii(7000.f, 7000.f);

	// ── RVO Collision-Avoidance 세팅 ────────────────
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->bUseRVOAvoidance  = true;
	MoveComp->SetAvoidanceEnabled(true);

	// ◆ AvoidanceConsiderationRadius
	//    └ NPC가 “얼마나 멀리서” 다른 에이전트를 감지하고 피할지 결정하는 레이더 반경(cm)
	//      • 값 ↑ : 멀리서 미리 비킴   • 값 ↓ : 꽤 가까워져야 피함
	MoveComp->AvoidanceConsiderationRadius = 400.f;

	// ◆ AvoidanceWeight
	//    └ 0.0 = 양보왕, 1.0 = 직진본능.
	//      회피 경로와 원래 경로 중 어느 쪽을 더 우선할지 가중치
	//      • 0.7 → “비켜주긴 하는데 내 길도 포기 안 함” 정도의 절충값
	MoveComp->AvoidanceWeight = 0.5f;

	// ◆ PushForceFactor (옵션)
	//    └ 실제로 캡슐끼리 맞닿았을 때 적용되는 물리 ‘밀기’ 힘(0~1)
	//      • 값 ↓ : 살살 밀어냄   • 값 ↑ : 몸싸움이 거칠어짐
	MoveComp->PushForceFactor          = 0.05f;
	MoveComp->TouchForceFactor         = 0.05f;

	// 모든 그룹을 피하고, 나는 그룹0에 속한다
	FNavAvoidanceMask All; All.SetFlagsDirectly(0xFFFFFFFF);
	FNavAvoidanceMask Me;  Me.ClearAll();  Me.SetGroup(0);

	MoveComp->SetAvoidanceGroupMask(Me);
	MoveComp->SetGroupsToAvoidMask(All);
}

void ADW_NPC_TownFolk::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADW_NPC_TownFolk::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	const float RandSpeed = FMath::RandRange(MinWalkSpeed, MaxWalkSpeed);
	MoveComp->MaxWalkSpeed    = RandSpeed;
	MoveComp->MaxAcceleration = RandSpeed * 4.f;
}

void ADW_NPC_TownFolk::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
#if WITH_EDITOR
	if (PatrolAnchorPoint)
	{
		DrawDebugSphere(
			GetWorld(),
			PatrolAnchorPoint->GetActorLocation(),
			RoamingRadius,
			24,  // 세그먼트 수 (원형 품질)
			FColor::Red,
			false,  // 영구적으로 그릴지 (false면 몇 초 후 사라짐)
			5.0f    // 지속 시간 (초)
		);
	}
#endif
}