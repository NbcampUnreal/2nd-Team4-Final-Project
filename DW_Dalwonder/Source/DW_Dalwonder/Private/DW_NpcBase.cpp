#include "DW_NpcBase.h"
#include "DW_GameInstance.h"
#include "Character/DW_CharacterBase.h"
#include "UI/Component/QuestDatabase.h"
#include "UI/Component/Manager/QuestManagerComponent.h"
#include "UI/Widget/DialogueWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Engine/DataTable.h"

// Sets default values
ADW_NpcBase::ADW_NpcBase()
{
}

void ADW_NpcBase::Interact_Implementation(AActor* Interactor)
{
    Super::Interact_Implementation(Interactor);

    if (!DialogueUIClass || !Interactor || !DialogueDataTable) return;

    APlayerController* PC = Cast<APlayerController>(Interactor->GetInstigatorController());
    if (PC)
    {
        UDialogueWidget* DialogueWidget = CreateWidget<UDialogueWidget>(PC, DialogueUIClass);
        if (DialogueWidget)
        {
            UQuestManagerComponent* QuestManager = Interactor->FindComponentByClass<UQuestManagerComponent>();
            TArray<FDialogueLine> FilteredLines = GetDialogueForQuestState(QuestManager);

            DialogueWidget->SetDialogueText(FilteredLines);

            if (bGivesQuest)
            {
                DialogueWidget->InitQuest(QuestID);
            }

            DialogueWidget->AddToViewport();
            PC->bShowMouseCursor = true;
            PC->SetInputMode(FInputModeUIOnly());

            FocusCameraOnNPC(Interactor);
        }
    }
}

TArray<FDialogueLine> ADW_NpcBase::GetDialogueForQuestState(UQuestManagerComponent* QuestManager) const
{
    TArray<FDialogueLine> Result;

    if (!DialogueDataTable) return Result;

    // 퀘스트 연동 여부 판단 기본값 None
    bool bHasQuest = QuestID != NAME_None;
    EQuestDialogueStage Stage = EQuestDialogueStage::None;

    if (bGivesQuest && bHasQuest && QuestManager)
    {
        if (UDW_GameInstance* DWGI = Cast<UDW_GameInstance>(GetGameInstance()))
        {
            if (UQuestDatabase* QuestDB = DWGI->QuestDatabase)
            {
                FQuestData QuestData = QuestDB->FindQuestByID(QuestID);

                if (QuestData.QuestID != NAME_None)  // 유효한지 확인
                {
                    if (QuestManager->IsQuestCompleted(QuestData))
                        Stage = EQuestDialogueStage::Completed;
                    else if (QuestManager->IsQuestAccepted(QuestData))
                        Stage = EQuestDialogueStage::InProgress;
                    else
                        Stage = EQuestDialogueStage::BeforeAccept;
                }
            }
        }
    }

    static const FString Context = TEXT("NPC Dialogue");
    TArray<FDialogueLine*> AllRows;
    DialogueDataTable->GetAllRows(Context, AllRows);

    for (const FDialogueLine* Row : AllRows)
    {
        if (!Row) continue;

        // 퀘스트가 있는 NPC → 해당 QuestID와 상태가 맞는 대사만
        if (bHasQuest)
        {
            if (Row->QuestID == QuestID && Row->DialogueStage == Stage)
            {
                Result.Add(*Row);
            }
        }
        // 퀘스트 없는 NPC → DialogueStage == None만 허용
        else if (Row->DialogueStage == EQuestDialogueStage::None)
        {
            Result.Add(*Row);
        }
    }

    return Result;
}

void ADW_NpcBase::FocusCameraOnNPC(AActor* PlayerActor)
{
    if (!NPCInteractionCamera) return;

    APlayerController* PC = Cast<APlayerController>(PlayerActor->GetInstigatorController());
    if (PC)
    {
        PC->SetViewTargetWithBlend(NPCInteractionCamera, 1.0f);
    }
}


