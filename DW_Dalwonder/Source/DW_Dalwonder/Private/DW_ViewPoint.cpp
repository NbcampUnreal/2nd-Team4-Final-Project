#include "DW_ViewPoint.h"
#include "LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

ADW_ViewPoint::ADW_ViewPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    SequenceActor = nullptr;
    SequencePlayer = nullptr;
    bHasInteracted = false;
    CachedPlayerController = nullptr;

    ElapsedTime = 0.0f;
}

void ADW_ViewPoint::BeginPlay()
{
    Super::BeginPlay();

    if (UStaticMeshComponent* MeshComp = FindComponentByClass<UStaticMeshComponent>())
    {
        InitialMeshLocation = MeshComp->GetRelativeLocation();
    }

    GetWorldTimerManager().SetTimer(FloatingTimerHandle, this, &ADW_ViewPoint::ToggleFloating, 0.01f, true);
}

void ADW_ViewPoint::ToggleFloating()
{
    UStaticMeshComponent* MeshComp = FindComponentByClass<UStaticMeshComponent>();
    if (!MeshComp) return;

    ElapsedTime += 0.01f;

    if (ElapsedTime > FloatingCycleDuration)
    {
        ElapsedTime = 0.0f;
    }

    float Alpha = ElapsedTime / FloatingCycleDuration;
    float Offset = FMath::Sin(Alpha * 2.0f * PI) * FloatingAmplitude;

    FVector NewLocation = InitialMeshLocation;
    NewLocation.Z += Offset;

    MeshComp->SetRelativeLocation(NewLocation);
}

void ADW_ViewPoint::Interact_Implementation(AActor* Interactor)
{
    if (bHasInteracted || !ViewPointSequence)
    {
        return;
    }

    bHasInteracted = true;

    FMovieSceneSequencePlaybackSettings PlaybackSettings;
    PlaybackSettings.bAutoPlay = true;

    SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
        GetWorld(),
        ViewPointSequence,
        PlaybackSettings,
        SequenceActor
    );

    if (SequencePlayer)
    {
        CachedPlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        if (CachedPlayerController)
        {
            CachedPlayerController->SetIgnoreLookInput(true);
            CachedPlayerController->SetIgnoreMoveInput(true);
            CachedPlayerController->SetInputMode(FInputModeUIOnly());
            CachedPlayerController->bShowMouseCursor = false;
        }

        SequencePlayer->OnFinished.AddDynamic(this, &ADW_ViewPoint::OnSequenceFinished);
        SequencePlayer->Play();
    }
}

void ADW_ViewPoint::OnSequenceFinished()
{
    if (CachedPlayerController)
    {
        CachedPlayerController->SetIgnoreLookInput(false);
        CachedPlayerController->SetIgnoreMoveInput(false);
        CachedPlayerController->SetInputMode(FInputModeGameOnly());
        CachedPlayerController->bShowMouseCursor = false;
    }

    bHasInteracted = false;
}
