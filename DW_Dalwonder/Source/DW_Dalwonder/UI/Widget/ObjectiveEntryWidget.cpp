// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ObjectiveEntryWidget.h"
#include "Components/TextBlock.h"

void UObjectiveEntryWidget::InitWithObjective(const FQuestObjective& QuestObjective)
{
    if (!ObjectiveText) return;

    FString ObjectiveTypeStr;
    switch (QuestObjective.ObjectiveType)
    {
    case EObjectiveType::Kill:
        ObjectiveTypeStr = TEXT("처치");
        break;
    case EObjectiveType::Collect:
        ObjectiveTypeStr = TEXT("수집");
        break;
    case EObjectiveType::Talk:
        ObjectiveTypeStr = TEXT("대화");
        break;
    case EObjectiveType::Move:
        ObjectiveTypeStr = TEXT("이동");
        break;
    case EObjectiveType::Custom:
        ObjectiveTypeStr = TEXT("기타");
        break;
    }

    const FString Display = FString::Printf(TEXT("[%s] %s (%d/%d)"),
        *ObjectiveTypeStr,
        *QuestObjective.TargetID.ToString(),
        QuestObjective.CurrentCount,
        QuestObjective.RequiredCount
    );

    ObjectiveText->SetText(FText::FromString(Display));
}
