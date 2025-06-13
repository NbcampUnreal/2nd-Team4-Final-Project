// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SavedGameWidget.h"
#include "UI/Widget/CustomButtonWidget.h"
#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"

void USavedGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
		BackButton->OnCustomClicked.AddDynamic(this, &USavedGameWidget::OnBackClicked);
}

void USavedGameWidget::OnBackClicked()
{
	if (ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->CloseLastPopupUI();  // 현재 위젯 닫기
	}
}
