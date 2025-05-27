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
	// Sets default values for this actor's properties
	ADW_ViewPoint();

    UPROPERTY(EditAnywhere, Category = "Sequence")
    ULevelSequence* ViewPointSequence;

protected:
    UPROPERTY()
    ALevelSequenceActor* SequenceActor;

    UPROPERTY()
    ULevelSequencePlayer* SequencePlayer;

    // 중복 방지 플래그
    bool bHasInteracted;

    // 연출 중 입력 제한 해제용 컨트롤러 캐시
    APlayerController* CachedPlayerController;

public:
    virtual void Interact_Implementation(AActor* Interactor) override;

    // 시퀀스 종료 후 입력 복구
    void OnSequenceFinished();
};