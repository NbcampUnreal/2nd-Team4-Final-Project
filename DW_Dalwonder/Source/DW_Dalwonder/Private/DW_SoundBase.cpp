#include "DW_SoundBase.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ADW_SoundBase::ADW_SoundBase()
{
    PrimaryActorTick.bCanEverTick = false;

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    RootComponent = AudioComponent;
    AudioComponent->bAutoActivate = false;
    AudioComponent->bAllowSpatialization = true;
}

void ADW_SoundBase::BeginPlay()
{
    Super::BeginPlay();

    PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (SoundAsset)
    {
        AudioComponent->SetSound(SoundAsset);
        AudioComponent->Play();
    }

    GetWorldTimerManager().SetTimer(
        VolumeUpdateTimer,
        this,
        &ADW_SoundBase::UpdateVolume,
        VolumeUpdateInterval,
        true
    );
}

void ADW_SoundBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetWorldTimerManager().ClearTimer(VolumeUpdateTimer);
}

float ADW_SoundBase::GetClosestDistanceToMarkingPoints() const
{
    if (!PlayerActor || MarkingPointActors.Num() == 0)
        return MaxRange;

    float Closest = MaxRange;

    for (AActor* MarkingActor : MarkingPointActors)
    {
        if (!MarkingActor) continue;

        float Dist = FVector::Dist(PlayerActor->GetActorLocation(), MarkingActor->GetActorLocation());
        if (Dist < Closest)
            Closest = Dist;
    }

    return Closest;
}

void ADW_SoundBase::UpdateVolume()
{
    float Distance = GetClosestDistanceToMarkingPoints();
    float Clamped = FMath::Clamp(Distance, 0.f, MaxRange);
    float Alpha = 1.f - (Clamped / MaxRange);
    float NewVolume = FMath::Lerp(MinVolume, MaxVolume, Alpha);
    AudioComponent->SetVolumeMultiplier(NewVolume);
}
