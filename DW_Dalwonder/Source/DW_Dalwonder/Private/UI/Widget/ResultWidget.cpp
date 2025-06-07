// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ResultWidget.h"
#include "Components/TextBlock.h"

void UResultWidget::SetResultText(const FString& InText)
{
	if (ResultText)
	{
		ResultText->SetText(FText::FromString(InText));
		ResultTextShadow->SetText(FText::FromString(InText));
	}
}

void UResultWidget::SetLetterSpacing(int32 Spacing)
{
	if (ResultTextShadow)
	{
		FSlateFontInfo FontInfo = ResultTextShadow->Font;
		FontInfo.LetterSpacing = Spacing;
		ResultTextShadow->SetFont(FontInfo);
	}
}

void UResultWidget::StartLetterSpacingAnimation()
{
	ElapsedTime = 0.f;
	InitialSpacing = 30;
	TargetSpacing = 100;
	LerpDuration = 1.5f;

	GetWorld()->GetTimerManager().SetTimer(LetterSpacingTimerHandle, this, &UResultWidget::UpdateLetterSpacing, 0.01f, true);
}

void UResultWidget::UpdateLetterSpacing()
{
	ElapsedTime += 0.01f;
	float Alpha = ElapsedTime / LerpDuration;

	float SmoothAlpha = FMath::InterpEaseOut(0.f, 1.f, Alpha, 2.f);

	int32 Spacing = FMath::Lerp(InitialSpacing, TargetSpacing, SmoothAlpha);
	SetLetterSpacing(Spacing);

	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(LetterSpacingTimerHandle);
	}
}
