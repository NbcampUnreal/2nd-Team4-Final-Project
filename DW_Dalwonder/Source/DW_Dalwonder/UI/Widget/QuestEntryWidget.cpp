// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/QuestEntryWidget.h"
#include "UI/Widget/QuestListWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UQuestEntryWidget::InitWithData(const FQuestData& InData, UQuestListWidget* ParentWidget)
{
    StoredQuestData = InData;
    OwnerListWidget = ParentWidget;

    if (QuestTitleText)
        QuestTitleText->SetText(InData.Title);

    if (QuestSummaryText)
        QuestSummaryText->SetText(InData.Description);
}

void UQuestEntryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ClickableButton)
    {
        ClickableButton->OnClicked.AddDynamic(this, &UQuestEntryWidget::HandleClick);
    }
}

void UQuestEntryWidget::HandleClick()
{
    if (OwnerListWidget)
    {
        OwnerListWidget->OnQuestSelected(StoredQuestData);
    }
}

