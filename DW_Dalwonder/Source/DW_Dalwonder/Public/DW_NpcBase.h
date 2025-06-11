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

    // UI 클래스 (인터페이스에서 사용)
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> DialogueUIClass;

    // 전역 UI 인스턴스
    UPROPERTY()
    UUserWidget* DialogueUIInstance;

    // 카메라 전환용 (자동화 or 수동 설정)
    UPROPERTY(EditAnywhere, Category = "Camera")
    ACameraActor* NPCInteractionCamera;

    // 카메라 제어 함수
    void FocusCameraOnNPC(AActor* PlayerActor);
};

