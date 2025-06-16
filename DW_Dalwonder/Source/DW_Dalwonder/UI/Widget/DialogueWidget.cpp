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

void UDialogueWidget::SetDialogueText(const TArray<FDialogueLine>& InDialogueLines)
{
    DialogueLines = InDialogueLines;
    CurrentDialogueIndex = 0;

    ShowNextDialogueLine(); // 첫 줄 바로 출력
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

        DialogueText->SetText(Line.DialogueText);
        SpeakerNameText->SetText(Line.SpeakerName);

        if (Line.bIsQuestDialogue)
        {
            bHasQuest = true;
            QuestID = Line.QuestID;

            // 안내 텍스트
            AcceptQuestButton->SetVisibility(ESlateVisibility::Visible);
            QuestQuestionText->SetText(FText::FromString(TEXT("수락")));
            QuestQuestionText->SetVisibility(ESlateVisibility::Visible);

            if (NexiDialogueText)
            {
                NexiDialogueText->SetText(FText::FromString(TEXT("거절")));
            }

            ++CurrentDialogueIndex;
            return;
        }

        ++CurrentDialogueIndex;

        if (NexiDialogueText)
        {
            NexiDialogueText->SetText(FText::FromString(TEXT("다음")));
        }
    }
    else
    {
        if (!bHasQuest)
        {
            RemoveFromParent();
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            PC->SetInputMode(FInputModeGameOnly());
            PC->bShowMouseCursor = false;
        }
    }

    //if (CurrentDialogueIndex < DialogueLines.Num())
    //{
    //    const FDialogueLine& Line = DialogueLines[CurrentDialogueIndex];

    //    if (DialogueText) DialogueText->SetText(Line.DialogueText);
    //    if (SpeakerNameText)  SpeakerNameText->SetText(Line.SpeakerName);

    //    // 마지막 줄이고 퀘스트 있음 → 버튼 이름 바꾸기
    //    if (Line.bIsQuestDialogue)
    //    {
    //        bHasQuest = true;
    //        QuestID = Line.QuestID;

    //        // 안내 텍스트 및 수락 버튼 표시
    //        AcceptQuestButton->SetVisibility(ESlateVisibility::Visible);
    //        QuestQuestionText->SetText(FText::FromString(TEXT("수락")));
    //        QuestQuestionText->SetVisibility(ESlateVisibility::Visible);

    //        // Next 버튼을 "거절" 역할로 전환
    //        if (NexiDialogueText)
    //        {
    //            NexiDialogueText->SetText(FText::FromString(TEXT("거절")));
    //        }
    //        ++CurrentDialogueIndex;
    //        // 여기서 대사 진행을 멈추고 버튼 클릭을 기다리기.
    //        return;
    //    }

    //    ++CurrentDialogueIndex;

    //    // 텍스트 복원 (혹시 이전에 "거절"로 바꿨던 것)
    //    if (NexiDialogueText)
    //    {
    //        NexiDialogueText->SetText(FText::FromString(TEXT("다음")));
    //    }
    //}
    //else
    //{
    //    // 마지막 줄 끝났고, 퀘스트도 없음 → 창 닫기
    //    if (!bHasQuest)
    //    {
    //        RemoveFromParent();

    //        APlayerController* PC = GetWorld()->GetFirstPlayerController();
    //        if (PC)
    //        {
    //            PC->SetInputMode(FInputModeGameOnly());
    //            PC->bShowMouseCursor = false;
    //        }
    //    }
    //}
}

void UDialogueWidget::InitQuest(FName InQuestID)
{
    QuestID = InQuestID;
    bHasQuest = true;

    if (UDW_GameInstance* GI = Cast<UDW_GameInstance>(GetGameInstance()))
    {
        if (UQuestDatabase* DB = GI->QuestDatabase)
        {
            CachedQuestData = DB->FindQuestByID(QuestID);
        }
    }
}

void UDialogueWidget::OnNextButtonClicked()
{
    // 퀘스트가 있는 상태에서 마지막 대사인 경우 → "거절" 역할 수행
    if (bHasQuest && CurrentDialogueIndex >= DialogueLines.Num())
    {
        // 거절 처리
        RemoveFromParent();

        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            PC->SetInputMode(FInputModeGameOnly());
            PC->bShowMouseCursor = false;
        }

        return;
    }

    ShowNextDialogueLine();
}

void UDialogueWidget::OnAcceptQuestClicked()
{
    if (!bHasQuest || CachedQuestData.QuestID == NAME_None) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(PC->GetPawn());

    if (Player && Player->QuestManager)
    {
        Player->QuestManager->AcceptQuest(CachedQuestData);

        // UI 닫기
        AcceptQuestButton->SetVisibility(ESlateVisibility::Collapsed);
        RemoveFromParent();
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;

        UE_LOG(LogTemp, Log, TEXT("퀘스트 [%s] 수락 완료"), *CachedQuestData.Title.ToString());
    }

    //if (DialogueLines.IsValidIndex(CurrentDialogueIndex - 1))
    //{
    //    const FDialogueLine& Line = DialogueLines[CurrentDialogueIndex - 1];

    //    if (Line.bIsQuestDialogue && Line.QuestID != NAME_None)
    //    {
    //        // 플레이어 캐릭터 접근
    //        APlayerController* PC = GetWorld()->GetFirstPlayerController();
    //        ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(PC->GetPawn());

    //        if (Player && Player->QuestManager)
    //        {
    //            // GameInstance에서 QuestDatabase 접근
    //            UDW_GameInstance* GI = Cast<UDW_GameInstance>(UGameplayStatics::GetGameInstance(this));
    //            if (GI && GI->QuestDatabase)
    //            {
    //                // QuestID로 퀘스트 정보 검색
    //                FQuestData Quest = GI->QuestDatabase->FindQuestByID(Line.QuestID);
    //                if (Quest.QuestID != NAME_None)
    //                {
    //                    // 퀘스트 수락
    //                    Player->QuestManager->AcceptQuest(Quest);

    //                    // UI 처리
    //                    AcceptQuestButton->SetVisibility(ESlateVisibility::Collapsed);
    //                    // 입력 모드 원래대로 복원
    //                    if (PC)
    //                    {
    //                        PC->SetInputMode(FInputModeGameOnly());
    //                        PC->bShowMouseCursor = false;
    //                        RemoveFromParent();
    //                    }

    //                    UE_LOG(LogTemp, Log, TEXT("퀘스트 [%s] 수락 완료"), *Quest.Title.ToString());
    //                }
    //                else
    //                {
    //                    UE_LOG(LogTemp, Warning, TEXT("퀘스트 ID '%s'를 QuestDatabase에서 찾을 수 없습니다."), *Line.QuestID.ToString());
    //                }
    //            }
    //            else
    //            {
    //                UE_LOG(LogTemp, Warning, TEXT("UDW_GameInstance 또는 QuestDatabase가 존재하지 않습니다."));
    //            }
    //        }
    //    }
    //}
}

