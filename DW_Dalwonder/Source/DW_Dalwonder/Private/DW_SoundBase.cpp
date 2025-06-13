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

// DW_SoundBase.cpp

void ADW_SoundBase::BeginPlay()
{
    Super::BeginPlay();

    PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (!MarkingPointTag.IsNone())
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), MarkingPointTag, FoundActors);
        MarkingPointActors = FoundActors;
    }

    if (SoundAsset)
    {
        AudioComponent->SetSound(SoundAsset);
        AudioComponent->Play();
    }

#if WITH_EDITOR
    UE_LOG(LogTemp, Warning, TEXT("총 마킹 포인트 수: %d"), MarkingPointActors.Num());
#endif

    for (int32 i = 0; i < MarkingPointActors.Num(); ++i)
    {
        if (MarkingPointActors[i])
        {
#if WITH_EDITOR
            UE_LOG(LogTemp, Warning, TEXT("마킹 %d: %s"), i, *MarkingPointActors[i]->GetName());
#endif
        }
    }




#if WITH_EDITOR
    for (AActor* MarkingActor : MarkingPointActors)
    {
        if (!MarkingActor) continue;

        DrawDebugSphere(
            GetWorld(),
            MarkingActor->GetActorLocation(),
            50000.0f,
            32,
            FColor::Red,
            true,
            10.0f,
            0,
            20.0f
        );
    }
#endif

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
