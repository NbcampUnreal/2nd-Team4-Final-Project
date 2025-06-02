#include "Character/DW_SwordBase.h"

ADW_SwordBase::ADW_SwordBase()
{
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	SwordTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStartPoint"));
	SwordTraceStartPoint->SetupAttachment(StaticMesh);
	SwordTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEndPoint"));
	SwordTraceEndPoint->SetupAttachment(StaticMesh);
}
