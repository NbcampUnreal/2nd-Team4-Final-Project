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
	LerpStartHP = CurrentDisplayHP;
	LerpTargetHP = NewHP;
	LerpElapsedTime = 0.f;

	GetWorld()->GetTimerManager().ClearTimer(HPBarLerpTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(
		HPBarLerpTimerHandle,
		this,
		&UBossHUDWidget::LerpHPBar,
		0.01f,
		true
	);
}

void UBossHUDWidget::LerpHPBar()
{
	LerpElapsedTime += 0.01f;

	float Alpha = FMath::Clamp(LerpElapsedTime / LerpDuration, 0.f, 1.f);
	float NewDisplayHP = FMath::Lerp(LerpStartHP, LerpTargetHP, Alpha);
	CurrentDisplayHP = NewDisplayHP;

	if (BossHPBar && MaxBossHP > 0)
	{
		float Ratio = FMath::Clamp(NewDisplayHP / MaxBossHP, 0.f, 1.f);
		BossHPBar->SetPercent(Ratio);
	}

	if (BossHPText)
	{
		FString Text = FString::Printf(TEXT("%.0f"), NewDisplayHP);
		BossHPText->SetText(FText::FromString(Text));
	}

	if (Alpha >= 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(HPBarLerpTimerHandle);
	}
}

