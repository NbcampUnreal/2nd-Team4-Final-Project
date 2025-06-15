// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Component/QuestDetailPanel.h"
#include "UI/Widget/ObjectiveEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

void UQuestDetailPanel::SetQuestInfo(const FQuestData& QuestData)
{
    if (QuestTitleText)
        QuestTitleText->SetText(QuestData.Title);

    if (QuestDescriptionText)
        QuestDescriptionText->SetText(QuestData.Description);

    if (ObjectiveList)
    {
        ObjectiveList->ClearChildren();
        UE_LOG(LogTemp, Log, TEXT("DetailPannel 호출됨"));
        for (const FQuestObjective& Objective : QuestData.Objectives)
        {
            UE_LOG(LogTemp, Log, TEXT("DetailPannel 호출됨2"));
            UE_LOG(LogTemp, Error, TEXT("ObjectiveEntryClass valid? %s | Outer: %s"),
                ObjectiveEntryClass ? TEXT("YES") : TEXT("NO"),
                *GetOuter()->GetName());
            if (ObjectiveEntryClass)
            {
                UE_LOG(LogTemp, Log, TEXT("DetailPannel 호출됨3"));
                UObjectiveEntryWidget* Entry = CreateWidget<UObjectiveEntryWidget>(this, ObjectiveEntryClass);
                if (Entry)
                {
                    Entry->InitWithObjective(Objective);
                    ObjectiveList->AddChild(Entry);
                }
            }
        }
    }
}

