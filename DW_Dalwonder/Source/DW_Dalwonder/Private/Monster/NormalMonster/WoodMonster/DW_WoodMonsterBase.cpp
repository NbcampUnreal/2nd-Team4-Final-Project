#include "Monster/NormalMonster/WoodMonster/DW_WoodMonsterBase.h"

ADW_WoodMonsterBase::ADW_WoodMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	TraceStart->SetupAttachment(GetMesh(), TEXT("index_01_l"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("index_01_r"));
}
