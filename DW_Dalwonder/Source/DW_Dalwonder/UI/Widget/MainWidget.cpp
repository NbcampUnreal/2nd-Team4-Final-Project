// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MainWidget.h"
#include "UI/Widget/CustomButtonWidget.h"
#include "UI/Widget/SavedGameWidget.h"
#include "DW_GmBase.h"
#include "DW_GmInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TimerManager.h"
#include "DW_GameInstance.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NewGameButton)
		NewGameButton->OnCustomClicked.AddDynamic(this, &UMainWidget::OnNewGameClicked);

	if (ContinueGameButton)
		ContinueGameButton->OnCustomClicked.AddDynamic(this, &UMainWidget::OnContinueGameClicked);

	if (OptionButton)
		OptionButton->OnCustomClicked.AddDynamic(this, &UMainWidget::OnOptionClicked);

	if (ExitButton)
		ExitButton->OnCustomClicked.AddDynamic(this, &UMainWidget::OnExitClicked);
}

void UMainWidget::OnNewGameClicked()
{
	//게임 시작 로직 (레벨이동? or UI없애기)
	UGameplayStatics::OpenLevel(this, "TestLoadingMap");
	RemoveFromParent();
}

void UMainWidget::OnContinueGameClicked()
{
	if (UDW_GameInstance* GameInstance = Cast<UDW_GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->LoadGameData(); // 게임로드
	}
}

void UMainWidget::OnOptionClicked()
{
	ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode && OptionMenuWidgetClass)
	{
		GameMode->ShowPopupUI(OptionMenuWidgetClass);
	}
}

void UMainWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
