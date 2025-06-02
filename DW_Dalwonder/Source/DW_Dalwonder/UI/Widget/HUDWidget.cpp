// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HUDWidget.h"
#include "Components/ProgressBar.h"

void UHUDWidget::NativeConstruct()
{
    
}

void UHUDWidget::UpdateHPBar(float Current, float Max)
{
    if (HPBar)
    {
        HPBar->SetPercent(Current / Max);
    }
}

void UHUDWidget::UpdateStaminaBar(float Current, float Max)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Current / Max);
	}
}
