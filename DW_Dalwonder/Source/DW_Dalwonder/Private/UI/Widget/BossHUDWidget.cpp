// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BossHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UBossHUDWidget::InitBossHUD(const FString& BossName, float MaxHP)
{
	if (BossNameText)
	{
		BossNameText->SetText(FText::FromString(BossName));
	}

	MaxBossHP = MaxHP;

	if (BossHPBar)
	{
		BossHPBar->SetPercent(1.0f);
	}
}

void UBossHUDWidget::UpdateHP(float NewHP)
{
	if (!BossHPBar || MaxBossHP <= 0.f) return;

	float NormalizedHP = FMath::Clamp(NewHP / MaxBossHP, 0.f, 1.f);
	BossHPBar->SetPercent(NormalizedHP);
}