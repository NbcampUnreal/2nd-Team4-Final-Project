// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Component/Button/BackButtonWidget.h"
#include "UI/Widget/CustomButtonWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DW_GmBase.h"

void UBackButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
	{
		BackButton->OnCustomClicked.AddDynamic(this, &UBackButtonWidget::HandleBackClicked);
	}
}

void UBackButtonWidget::HandleBackClicked()
{
	if (ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->CloseLastPopupUI();
	}
}
