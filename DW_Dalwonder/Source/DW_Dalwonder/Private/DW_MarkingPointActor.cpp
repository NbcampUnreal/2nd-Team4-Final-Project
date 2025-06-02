#include "DW_MarkingPointActor.h"

ADW_MarkingPointActor::ADW_MarkingPointActor()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetActorHiddenInGame(true); // 월드에서 보이지 않도록 설정 (필요시)
}
