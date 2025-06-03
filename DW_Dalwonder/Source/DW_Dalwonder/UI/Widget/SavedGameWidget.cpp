// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SavedGameWidget.h"
#include "UI/Widget/CustomButtonWidget.h"
#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"
#include "DW_GameInstance.h"

void USavedGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
		BackButton->OnCustomClicked.AddDynamic(this, &USavedGameWidget::OnBackClicked);

	// 저장게임 불러오기
	if (SaveData1)
		SaveData1->OnCustomClicked.AddDynamic(this, &USavedGameWidget::OnSaveSlot1Clicked);
	if (SaveData2)
		SaveData2->OnCustomClicked.AddDynamic(this, &USavedGameWidget::OnSaveSlot2Clicked);
	if (SaveData3)
		SaveData3->OnCustomClicked.AddDynamic(this, &USavedGameWidget::OnSaveSlot3Clicked);
	if (SaveData4)
		SaveData4->OnCustomClicked.AddDynamic(this, &USavedGameWidget::OnSaveSlot4Clicked);
}

void USavedGameWidget::SetMode(ESaveGameWidgetMode InMode)
{
	WidgetMode = InMode;

	if (TitleText)
	{
		FText DisplayText = (WidgetMode == ESaveGameWidgetMode::Save)
			? FText::FromString(TEXT("저장하기"))
			: FText::FromString(TEXT("불러오기"));

		TitleText->SetText(DisplayText);
	}
}

void USavedGameWidget::OnBackClicked()
{
	if (ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->CloseLastPopupUI();  // 현재 위젯 닫기
	}
}

void USavedGameWidget::OnSaveSlot1Clicked() { HandleSlotClicked(TEXT("DW_SaveSlot1")); }
void USavedGameWidget::OnSaveSlot2Clicked() { HandleSlotClicked(TEXT("DW_SaveSlot2")); }
void USavedGameWidget::OnSaveSlot3Clicked() { HandleSlotClicked(TEXT("DW_SaveSlot3")); }
void USavedGameWidget::OnSaveSlot4Clicked() { HandleSlotClicked(TEXT("DW_SaveSlot4")); }

void USavedGameWidget::HandleSlotClicked(FString SlotName)
{
	if (UDW_GameInstance* GI = Cast<UDW_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (WidgetMode == ESaveGameWidgetMode::Save)
		{
			//GI->SaveGameDataToSlot(SlotName);
			UE_LOG(LogTemp, Log, TEXT("[SavedGameWidget] 저장됨: %s"), *SlotName);
		}
		else if (WidgetMode == ESaveGameWidgetMode::Load)
		{
			//GI->LoadGameDataFromSlot(SlotName);
			UE_LOG(LogTemp, Log, TEXT("[SavedGameWidget] 불러오기 완료: %s"), *SlotName);
		}
	}
}

