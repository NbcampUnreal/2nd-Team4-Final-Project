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

    // UI 클래스 (블루프린트에서 지정)
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> DialogueUIClass;

    // 현재 UI 인스턴스
    UPROPERTY()
    UUserWidget* DialogueUIInstance;

    // 카메라 전환용 (시퀀서 or 직접 제어용)
    UPROPERTY(EditAnywhere, Category = "Camera")
    ACameraActor* NPCInteractionCamera;

    // 카메라 연출 함수
    void FocusCameraOnNPC(AActor* PlayerActor);
};

