#include "StarEffectActor.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"

AStarEffectActor::AStarEffectActor()
{
    PrimaryActorTick.bCanEverTick = false;

    NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    RootComponent = NiagaraComp;
}

void AStarEffectActor::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(
        DestroyTimerHandle,
        this,
        &AStarEffectActor::DestroyEffectActor,
        1.0f,
        false
    );
}

void AStarEffectActor::DestroyEffectActor()
{
    Destroy();
}

void AStarEffectActor::InitEffect(UNiagaraSystem* NiagaraSystem, FVector Location, FRotator Rotation)
{
    SetActorLocation(Location);
    SetActorRotation(Rotation);

    if (NiagaraSystem && NiagaraComp)
    {
        NiagaraComp->SetAsset(NiagaraSystem);
        NiagaraComp->Activate(true);
    }
}
