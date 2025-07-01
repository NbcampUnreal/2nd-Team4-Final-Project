#include "NPC/DW_NPC_TownFolk.h"

#include "NavigationInvokerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NeutralityNPC/NPC_Controller.h"

ADW_NPC_TownFolk::ADW_NPC_TownFolk()
{
	{
		AIControllerClass = ANPC_Controller::StaticClass();
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
	NavInvokerComp = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));
	NavInvokerComp->SetGenerationRadii(7000.f, 7000.f);
}

void ADW_NPC_TownFolk::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// void ADW_NPC_TownFolk::OnConstruction(const FTransform& Transform)
// {
// 	Super::OnConstruction(Transform);
// 	
// 	UE_LOG(LogTemp, Warning, TEXT("OnConstruction Called!"));
//
// #if WITH_EDITOR
// 	if (PatrolAnchorPoint)
// 	{
// 		DrawDebugSphere(
// 			GetWorld(),
// 			PatrolAnchorPoint->GetActorLocation(),
// 			RoamingRadius,
// 			24,  // 세그먼트 수 (원형 품질)
// 			FColor::Red,
// 			false,  // 영구적으로 그릴지 (false면 몇 초 후 사라짐)
// 			5.0f    // 지속 시간 (초)
// 		);
// 	}
// #endif
// }