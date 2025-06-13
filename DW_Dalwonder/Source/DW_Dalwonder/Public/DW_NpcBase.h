#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_InteractItemBase.h"
#include "UI/Component/Struct/DialogueLine.h"
#include "DW_NpcBase.generated.h"

class UUserWidget;
class ACameraActor;

UCLASS()
class DW_DALWONDER_API ADW_NpcBase : public ADW_InteractItemBase
{
	GENERATED_BODY()
	
public:
    ADW_NpcBase();

public:
    // 대사 정보
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<FDialogueLine> DialogueLines;

    // 퀘스트 ID (이 NPC가 주는 퀘스트)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FName QuestID;

    // 퀘스트 대사 포함 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    bool bGivesQuest = false;

protected:
    virtual void Interact_Implementation(AActor* Interactor) override;

    // 카메라 제어 함수
    void FocusCameraOnNPC(AActor* PlayerActor);

protected:
    // UI 클래스 (인터페이스에서 사용)
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> DialogueUIClass;

    // 전역 UI 인스턴스
    UPROPERTY()
    UUserWidget* DialogueUIInstance;

    // 카메라 전환용 (자동화 or 수동 설정)
    UPROPERTY(EditAnywhere, Category = "Camera")
    ACameraActor* NPCInteractionCamera;
};

