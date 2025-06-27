#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UI/Component/Struct/DialogueLine.h"
#include "UI/Component/Struct/QuestData.h"
#include "DW_InteractInterface.h"
#include "Components/WidgetComponent.h"
#include "DW_NpcBase.generated.h"

class UUserWidget;
class ACameraActor;

UCLASS()
class DW_DALWONDER_API ADW_NpcBase : public ACharacter, public IDW_InteractInterface
{
	GENERATED_BODY()

public:
	ADW_NpcBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	USkeletalMeshComponent* MeshComponent;

	FORCEINLINE USkeletalMeshComponent* GetNpcMesh() const { return MeshComponent; }

	// 위젯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	UWidgetComponent* InteractionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	UDataTable* DialogueDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName QuestID;

  /** NPC이름 (이 NPC가 주는 퀘스트) */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
  FText Name;

  /** 퀘스트 대사 포함 여부 */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
  bool bGivesQuest = false;

protected:
	virtual void BeginPlay() override;

	// 인터페이스 구현
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual void ShowInteractionWidget_Implementation() override;
	virtual void HideInteractionWidget_Implementation() override;

	void FocusCameraOnNPC(AActor* PlayerActor);
	TArray<FDialogueLine> GetDialogueForQuestState(class UQuestManagerComponent* QuestManager) const;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DialogueUIClass;

	UPROPERTY()
	UUserWidget* DialogueUIInstance;

	UPROPERTY(EditAnywhere, Category = "Camera")
	ACameraActor* NPCInteractionCamera;

};
