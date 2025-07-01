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
	NavInvokerComp->SetGenerationRadii(5000.f, 5000.f);
}

void ADW_NPC_TownFolk::BeginPlay()
{
	Super::BeginPlay();
}
