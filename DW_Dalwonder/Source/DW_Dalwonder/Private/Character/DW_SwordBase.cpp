#include "Character/DW_SwordBase.h"

ADW_SwordBase::ADW_SwordBase()
{
	SwordTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStartPoint"));
	SwordTraceStartPoint->SetupAttachment(MeshComponent);
	SwordTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEndPoint"));
	SwordTraceEndPoint->SetupAttachment(MeshComponent);

	Tags.Add("Weapon");
}

void ADW_SwordBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SwordTraceStartPoint->AttachToComponent(MeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SwordTraceEndPoint->AttachToComponent(MeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
