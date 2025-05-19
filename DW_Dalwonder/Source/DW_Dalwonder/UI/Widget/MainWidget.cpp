// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MainWidget.h"
#include "UI/Widget/CustomButtonWidget.h"
#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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
	ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode && CharacterWidgetClass)
	{
		GameMode->SwitchUI(CharacterWidgetClass);
		GameMode->ClosePopupUI(this);
	}
}

void UMainWidget::OnContinueGameClicked()
{
	ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode && SavedGameWidgetClass)
	{
		GameMode->SwitchUI(SavedGameWidgetClass);
		GameMode->ClosePopupUI(this);
	}
}

void UMainWidget::OnOptionClicked()
{
	ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode && OptionMenuWidgetClass)
	{
		GameMode->SwitchUI(OptionMenuWidgetClass);
		GameMode->ClosePopupUI(this);
	}
}

void UMainWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
