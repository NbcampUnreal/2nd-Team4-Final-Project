#include "DW_ViewPoint.h"
#include "LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ADW_ViewPoint::ADW_ViewPoint()
{
    PrimaryActorTick.bCanEverTick = false;
    SequenceActor = nullptr;
    SequencePlayer = nullptr;
    bHasInteracted = false;
    CachedPlayerController = nullptr;
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
        // 연출 중 입력 제한
        CachedPlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        if (CachedPlayerController)
        {
            CachedPlayerController->SetIgnoreLookInput(true);
            CachedPlayerController->SetIgnoreMoveInput(true);
            CachedPlayerController->SetInputMode(FInputModeUIOnly());
            CachedPlayerController->bShowMouseCursor = false;
        }

        // 시퀀스 재생 및 종료 이벤트 바인딩
        SequencePlayer->OnFinished.AddDynamic(this, &ADW_ViewPoint::OnSequenceFinished);
        SequencePlayer->Play();
    }
}

void ADW_ViewPoint::OnSequenceFinished()
{
    // 입력 복원
    if (CachedPlayerController)
    {
        CachedPlayerController->SetIgnoreLookInput(false);
        CachedPlayerController->SetIgnoreMoveInput(false);
        CachedPlayerController->SetInputMode(FInputModeGameOnly());
        CachedPlayerController->bShowMouseCursor = false;
    }
}