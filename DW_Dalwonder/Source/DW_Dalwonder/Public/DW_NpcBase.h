#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_InteractItemBase.h"
#include "UI/Component/Struct/DialogueLine.h"
#include "UI/Component/Struct/QuestData.h"
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
    /** 대화 데이터 테이블 (NPC마다 할당) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    UDataTable* DialogueDataTable;

    /** 퀘스트 ID (이 NPC가 주는 퀘스트) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FName QuestID;

    /** 퀘스트 대사 포함 여부 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    bool bGivesQuest = false;

protected:
    virtual void Interact_Implementation(AActor* Interactor) override;
    void FocusCameraOnNPC(AActor* PlayerActor);

    /** 퀘스트 진행 상태에 맞는 대사 필터링 */
    TArray<FDialogueLine> GetDialogueForQuestState(class UQuestManagerComponent* QuestManager) const;

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

