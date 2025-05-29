#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_InteractItemBase.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "DW_ViewPoint.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_ViewPoint : public ADW_InteractItemBase
{
    GENERATED_BODY()

public:
    ADW_ViewPoint();

    UPROPERTY(EditAnywhere, Category = "Sequence")
    ULevelSequence* ViewPointSequence;

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    ALevelSequenceActor* SequenceActor;

    UPROPERTY()
    ULevelSequencePlayer* SequencePlayer;

    bool bHasInteracted;
    APlayerController* CachedPlayerController;

    FTimerHandle FloatingTimerHandle;

    FVector InitialMeshLocation;
    float ElapsedTime;

    UPROPERTY(EditAnywhere, Category = "Floating")
    float FloatingCycleDuration = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Floating")
    float FloatingAmplitude = 20.0f;

    void ToggleFloating();

public:
    virtual void Interact_Implementation(AActor* Interactor) override;

    UFUNCTION()
    void OnSequenceFinished();
};
