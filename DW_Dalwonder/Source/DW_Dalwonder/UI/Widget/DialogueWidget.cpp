// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/DialogueWidget.h"
#include "Components/TextBlock.h"

void UDialogueWidget::SetDialogueText(const FText& InText)
{
    if (DialogueText)
    {
        DialogueText->SetText(InText);
    }
}

void UDialogueWidget::SetSpeakerName(const FText& InName)
{
    if (SpeakerNameText)
    {
        SpeakerNameText->SetText(InName);
    }
}

