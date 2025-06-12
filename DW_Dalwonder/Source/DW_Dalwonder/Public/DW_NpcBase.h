#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_InteractItemBase.h"
#include "DW_NpcBase.generated.h"

class UUserWidget;
class ACameraActor;

UCLASS()
class DW_DALWONDER_API ADW_NpcBase : public ADW_InteractItemBase
{
	GENERATED_BODY()
	
public:
    ADW_NpcBase();

protected:
    virtual void Interact_Implementation(AActor* Interactor) override;

    // UI Ŭ���� (�������Ʈ���� ����)
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> DialogueUIClass;

    // ���� UI �ν��Ͻ�
    UPROPERTY()
    UUserWidget* DialogueUIInstance;

    // ī�޶� ��ȯ�� (������ or ���� �����)
    UPROPERTY(EditAnywhere, Category = "Camera")
    ACameraActor* NPCInteractionCamera;

    // ī�޶� ���� �Լ�
    void FocusCameraOnNPC(AActor* PlayerActor);
};

