#include "Character/DW_SwordBase.h"

ADW_SwordBase::ADW_SwordBase()
{
	SwordTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStartPoint"));
	SwordTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEndPoint"));
}

void ADW_SwordBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SwordTraceStartPoint->AttachToComponent(MeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SwordTraceEndPoint->AttachToComponent(MeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
