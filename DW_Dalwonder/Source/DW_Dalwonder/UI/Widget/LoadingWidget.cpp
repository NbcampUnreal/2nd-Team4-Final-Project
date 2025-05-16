// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/LoadingWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULoadingWidget::SetLoadingProgress(float InPercent)
{
    if (LoadingProgressBar)
    {
        LoadingProgressBar->SetPercent(InPercent);
    }

    if (LoadingText)
    {
        FString ProgressText = FString::Printf(TEXT("%.0f%%"), InPercent * 100);
        LoadingText->SetText(FText::FromString(ProgressText));
    }
}

void ULoadingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 초기화 시 로딩바 0%
    if (LoadingProgressBar)
    {
        LoadingProgressBar->SetPercent(0.f);
    }
}
