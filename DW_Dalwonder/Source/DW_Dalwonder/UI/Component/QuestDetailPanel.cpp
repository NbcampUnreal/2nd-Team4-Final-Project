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

        for (const FQuestObjective& Objective : QuestData.Objectives)
        {
            if (ObjectiveEntryClass)
            {
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

