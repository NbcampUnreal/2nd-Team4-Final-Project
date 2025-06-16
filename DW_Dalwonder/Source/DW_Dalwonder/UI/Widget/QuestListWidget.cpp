// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/QuestListWidget.h"
#include "UI/Widget/QuestEntryWidget.h"
#include "UI/Widget/ObjectiveEntryWidget.h"
#include "UI/Component/QuestDetailPanel.h"
#include "UI/Component/Manager/QuestManagerComponent.h"
#include "Components/Button.h"
#include "Components/ExpandableArea.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

void UQuestListWidget::NativeConstruct()
{
    Super::NativeConstruct();

    QuestManager = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)
        ->FindComponentByClass<UQuestManagerComponent>();

    RefreshQuestList();
}

void UQuestListWidget::RefreshQuestList()
{
    if (!QuestManager || !QuestEntryClass) return;

    InProgressList->ClearChildren();
    CompletedList->ClearChildren();

    const TMap<FName, FQuestData>& Active = QuestManager->GetActiveQuests();
    const TMap<FName, FQuestData>& Completed = QuestManager->GetCompletedQuests();

    // 진행 중 퀘스트 추가
    for (const auto& Pair : Active)
    {
        UQuestEntryWidget* Entry = CreateWidget<UQuestEntryWidget>(this, QuestEntryClass);
        Entry->InitWithData(Pair.Value, this);  // 부모 전달
        InProgressList->AddChild(Entry);
    }

    // 완료 퀘스트 추가
    for (const auto& Pair : Completed)
    {
        UQuestEntryWidget* Entry = CreateWidget<UQuestEntryWidget>(this, QuestEntryClass);
        Entry->InitWithData(Pair.Value, this);  // 부모 전달
        CompletedList->AddChild(Entry);
    }
}

void UQuestListWidget::OnQuestSelected(const FQuestData& QuestData)
{
    if (QuestDetailPanel)
    {
        //여기서 지정해주기 안해주면 오류남(Null처리당함)
        QuestDetailPanel->ObjectiveEntryClass = ObjectiveEntryWidgetClass;
        QuestDetailPanel->SetQuestInfo(QuestData);
    }
}

