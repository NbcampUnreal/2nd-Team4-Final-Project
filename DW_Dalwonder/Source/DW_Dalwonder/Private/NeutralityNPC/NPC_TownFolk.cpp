#include "NeutralityNPC/NPC_TownFolk.h"
#include "NeutralityNPC/NPC_Controller.h"

ANPC_TownFolk::ANPC_TownFolk()
{
	AIControllerClass = ANPC_Controller::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
