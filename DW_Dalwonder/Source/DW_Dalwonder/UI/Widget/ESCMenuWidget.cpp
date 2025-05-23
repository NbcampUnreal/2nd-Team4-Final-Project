// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ESCMenuWidget.h"
#include "UI/Widget/CustomButtonWidget.h"
#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UESCMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 이벤트 바인딩
    if (InfoButton)
        InfoButton->OnCustomClicked.AddDynamic(this, &UESCMenuWidget::OnInfoClicked);

    if (InventoryButton)
        InventoryButton->OnCustomClicked.AddDynamic(this, &UESCMenuWidget::OnInventoryClicked);

    if (OptionButton)
        OptionButton->OnCustomClicked.AddDynamic(this, &UESCMenuWidget::OnOptionClicked);

    if (ExitButton)
        ExitButton->OnCustomClicked.AddDynamic(this, &UESCMenuWidget::OnExitClicked);
}

void UESCMenuWidget::OnInfoClicked()
{
    ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
    if (GameMode && PlayerInfoWidgetClass)
    {
        GameMode->SwitchUI(PlayerInfoWidgetClass);
        GameMode->ClosePopupUI(this);
    }
}

void UESCMenuWidget::OnInventoryClicked()
{
    ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
    if (GameMode && InventoryWidgetClass)
    {
        GameMode->SwitchUI(InventoryWidgetClass);
        GameMode->ClosePopupUI(this);
    }
}

void UESCMenuWidget::OnOptionClicked()
{
    ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
    if (GameMode && OptionMenuWidgetClass)
    {
        GameMode->SwitchUI(OptionMenuWidgetClass);
        GameMode->ClosePopupUI(this);
    }
}

void UESCMenuWidget::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
