// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/LoadingWidget.h"

void ULoadingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 5초마다 랜덤 팁 업데이트
    GetWorld()->GetTimerManager().SetTimer(
        TipTimerHandle,
        this,
        &ULoadingWidget::UpdateRandomTip,
        5.0f,
        true
    );

    // 처음 한 번 바로 실행
    UpdateRandomTip();
}

void ULoadingWidget::UpdateRandomTip()
{
    if (TipList.Num() == 0) return;

    int32 Index = FMath::RandRange(0, TipList.Num() - 1);
    FString SelectedTip = TipList[Index];

    OnTipUpdated(SelectedTip);
}
