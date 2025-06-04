#include "DW_MarkingPointActor.h"

ADW_MarkingPointActor::ADW_MarkingPointActor()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetActorHiddenInGame(true);

    Tags.Add(FName("Marking_Fire"));
}