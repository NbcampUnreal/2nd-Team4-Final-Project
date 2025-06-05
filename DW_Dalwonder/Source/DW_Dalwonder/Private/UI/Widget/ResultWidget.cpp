// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ResultWidget.h"
#include "Components/TextBlock.h"

void UResultWidget::SetResultText(const FString& InText)
{
	if (ResultText)
	{
		ResultText->SetText(FText::FromString(InText));
	}
}