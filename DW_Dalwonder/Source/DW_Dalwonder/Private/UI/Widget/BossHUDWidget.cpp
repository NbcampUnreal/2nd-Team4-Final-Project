// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BossHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UBossHUDWidget::InitBossHUD(const FString& BossName, float MaxHP)
{
	MaxBossHP = MaxHP;
	
	if (BossNameText)
	{
		BossNameText->SetText(FText::FromString(BossName));
	}

	if (BossHPText)
	{
		FString Text = FString::Printf(TEXT("%.0f"), MaxBossHP);
		BossHPText->SetText(FText::FromString(Text));
	}

	if (BossHPBar)
	{
		BossHPBar->SetPercent(1.0f);
	}
}

void UBossHUDWidget::UpdateHP(float NewHP)
{
	if (MaxBossHP <= 0.f) return;

	float Ratio = FMath::Clamp(NewHP / MaxBossHP, 0.f, 1.f);

	if (BossHPBar)
	{
		BossHPBar->SetPercent(Ratio);
	}

	if (BossHPText)
	{
		FString Text = FString::Printf(TEXT("%.0f"), NewHP);
		BossHPText->SetText(FText::FromString(Text));
	}
}