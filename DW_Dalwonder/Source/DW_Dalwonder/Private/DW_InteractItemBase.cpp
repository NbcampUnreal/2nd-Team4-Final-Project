#include "DW_InteractItemBase.h"

ADW_InteractItemBase::ADW_InteractItemBase()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
}

void ADW_InteractItemBase::Interact_Implementation(AActor* Interactor)
{
    UE_LOG(LogTemp, Warning, TEXT("기본 아이템이 상호작용됨"));
}
