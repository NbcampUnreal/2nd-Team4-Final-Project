#include "DW_MarkingPointActor.h"

ADW_MarkingPointActor::ADW_MarkingPointActor()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetActorHiddenInGame(true); // ���忡�� ������ �ʵ��� ���� (�ʿ��)
}
