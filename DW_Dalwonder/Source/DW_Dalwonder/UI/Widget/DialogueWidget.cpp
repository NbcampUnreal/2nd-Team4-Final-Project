// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/DialogueWidget.h"
#include "Character/DW_CharacterBase.h"
#include "DW_GameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Component/QuestDatabase.h"
#include "UI/Component/Manager/QuestManagerComponent.h"

void UDialogueWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (NextDialogueButton)
    {
        NextDialogueButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnNextButtonClicked);
    }

    if (AcceptQuestButton)
    {
        AcceptQuestButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnAcceptQuestClicked);
    }
}

void UDialogueWidget::SetDialogueText(const FText& InText)
{
    if (CurrentDialogueIndex < DialogueLines.Num())
    {
        const FDialogueLine& Line = DialogueLines[CurrentDialogueIndex];
        SetSpeakerName(Line.SpeakerName);
        SetDialogueText(Line.DialogueText);

        if (Line.bIsQuestDialogue)
        {
            // 퀘스트 관련 대사라면 버튼 표시
            if (AcceptQuestButton)
            {
                AcceptQuestButton->SetVisibility(ESlateVisibility::Visible);
            }
        }
        else
        {
            if (AcceptQuestButton)
            {
                AcceptQuestButton->SetVisibility(ESlateVisibility::Collapsed);
            }
        }

        CurrentDialogueIndex++;
    }
    else
    {
        // 대사 끝났을 때 처리
        RemoveFromParent(); // 혹은 UI 숨기기
    }
}

void UDialogueWidget::SetSpeakerName(const FText& InName)
{
    if (SpeakerNameText)
    {
        SpeakerNameText->SetText(InName);
    }
}

void UDialogueWidget::ShowNextDialogueLine()
{
    if (CurrentDialogueIndex < DialogueLines.Num())
    {
        const FDialogueLine& Line = DialogueLines[CurrentDialogueIndex];

        // 텍스트 및 화자 설정
        if (SpeakerNameText) SpeakerNameText->SetText(Line.SpeakerName);
        if (DialogueText) DialogueText->SetText(Line.DialogueText);

        // 이 대사가 퀘스트 안내용이라면
        if (Line.bIsQuestDialogue)
        {
            bHasQuest = true;
            QuestID = Line.QuestID;

            // 퀘스트 수락 버튼/안내 표시
            if (DialogueText)
            {
                DialogueText->SetText(FText::FromString("퀘스트를 수락하시겠습니까?"));
                DialogueText->SetVisibility(ESlateVisibility::Visible);
            }

            if (AcceptQuestButton)
            {
                AcceptQuestButton->SetVisibility(ESlateVisibility::Visible);
            }
        }

        ++CurrentDialogueIndex;
    }
    else
    {
        // 대사 끝났고 퀘스트 수락도 없다면 종료
        if (!bHasQuest)
        {
            RemoveFromParent();
        }
    }
}

void UDialogueWidget::InitQuest(FName InQuestID)
{
    QuestID = InQuestID;
    bHasQuest = true;
}

void UDialogueWidget::OnNextButtonClicked()
{
    ShowNextDialogueLine();
}

void UDialogueWidget::OnAcceptQuestClicked()
{
    if (DialogueLines.IsValidIndex(CurrentDialogueIndex - 1))
    {
        const FDialogueLine& Line = DialogueLines[CurrentDialogueIndex - 1];

        if (Line.bIsQuestDialogue && Line.QuestID != NAME_None)
        {
            // 플레이어 캐릭터 접근
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(PC->GetPawn());

            if (Player && Player->QuestManager)
            {
                // GameInstance에서 QuestDatabase 접근
                UDW_GameInstance* GI = Cast<UDW_GameInstance>(UGameplayStatics::GetGameInstance(this));
                if (GI && GI->QuestDatabase)
                {
                    // QuestID로 퀘스트 정보 검색
                    FQuestData Quest = GI->QuestDatabase->FindQuestByID(Line.QuestID);
                    if (Quest.QuestID != NAME_None)
                    {
                        // 퀘스트 수락
                        Player->QuestManager->AcceptQuest(Quest);

                        // UI 처리
                        AcceptQuestButton->SetVisibility(ESlateVisibility::Collapsed);
                        RemoveFromParent();

                        UE_LOG(LogTemp, Log, TEXT("퀘스트 [%s] 수락 완료"), *Quest.Title.ToString());
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("퀘스트 ID '%s'를 QuestDatabase에서 찾을 수 없습니다."), *Line.QuestID.ToString());
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("UDW_GameInstance 또는 QuestDatabase가 존재하지 않습니다."));
                }
            }
        }
    }
}

