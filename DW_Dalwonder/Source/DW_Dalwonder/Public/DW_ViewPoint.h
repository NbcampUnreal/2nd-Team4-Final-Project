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

    // �ߺ� ���� �÷���
    bool bHasInteracted;

    // ���� �� �Է� ���� ������ ��Ʈ�ѷ� ĳ��
    APlayerController* CachedPlayerController;

public:
    virtual void Interact_Implementation(AActor* Interactor) override;

    // ������ ���� �� �Է� ����
    void OnSequenceFinished();
};