#include "Character/DW_SwordBase.h"

ADW_SwordBase::ADW_SwordBase()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneRoot);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	SwordTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStartPoint"));
	SwordTraceStartPoint->SetupAttachment(StaticMesh);
	SwordTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEndPoint"));
	SwordTraceEndPoint->SetupAttachment(StaticMesh);
}
