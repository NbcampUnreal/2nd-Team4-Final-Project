#include "Monster/NormalMonster/WoodMonster/DW_WoodMonsterBase.h"

ADW_WoodMonsterBase::ADW_WoodMonsterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	TraceStart->SetupAttachment(GetMesh());
	TraceEnd->SetupAttachment(GetMesh());
}
