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
        // ���� �� �Է� ����
        CachedPlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        if (CachedPlayerController)
        {
            CachedPlayerController->SetIgnoreLookInput(true);
            CachedPlayerController->SetIgnoreMoveInput(true);
            CachedPlayerController->SetInputMode(FInputModeUIOnly());
            CachedPlayerController->bShowMouseCursor = false;
        }

        // ������ ��� �� ���� �̺�Ʈ ���ε�
        SequencePlayer->OnFinished.AddDynamic(this, &ADW_ViewPoint::OnSequenceFinished);
        SequencePlayer->Play();
    }
}

void ADW_ViewPoint::OnSequenceFinished()
{
    // �Է� ����
    if (CachedPlayerController)
    {
        CachedPlayerController->SetIgnoreLookInput(false);
        CachedPlayerController->SetIgnoreMoveInput(false);
        CachedPlayerController->SetInputMode(FInputModeGameOnly());
        CachedPlayerController->bShowMouseCursor = false;
    }
}