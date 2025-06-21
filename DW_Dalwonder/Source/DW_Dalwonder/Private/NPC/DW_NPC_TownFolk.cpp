#include "NPC/DW_NPC_TownFolk.h"
#include "NeutralityNPC/NPC_Controller.h"

ADW_NPC_TownFolk::ADW_NPC_TownFolk()
{
	{
		AIControllerClass = ANPC_Controller::StaticClass();
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
}
