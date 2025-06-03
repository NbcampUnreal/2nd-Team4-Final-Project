// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MainWidget.h"
#include "UI/Widget/CustomButtonWidget.h"
#include "UI/Widget/SavedGameWidget.h"
#include "DW_GmBase.h"
#include "DW_GmInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TimerManager.h"

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
	ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode && SavedGameWidgetClass)
	{
		// ShowPopupUI() 호출 → UUserWidget* 반환
		UUserWidget* CreatedWidget = GameMode->ShowPopupUI(SavedGameWidgetClass);

		// SavedGameWidget으로 캐스팅하여 Load 모드 설정
		if (USavedGameWidget* LoadWidget = Cast<USavedGameWidget>(CreatedWidget))
		{
			LoadWidget->SetMode(ESaveGameWidgetMode::Load);
		}
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
