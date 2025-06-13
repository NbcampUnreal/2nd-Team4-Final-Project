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

    if (SkillTreeButton)
        SkillTreeButton->OnCustomClicked.AddDynamic(this, &UESCMenuWidget::OnSkillTreeClicked);

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
        GameMode->ShowPopupUI(PlayerInfoWidgetClass);

    }
}

void UESCMenuWidget::OnSkillTreeClicked()
{
    ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
    if (GameMode && SkillTreeWidgetClass)
    {
        GameMode->ShowPopupUI(SkillTreeWidgetClass);
    }
}

void UESCMenuWidget::OnInventoryClicked()
{
    ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
    if (GameMode && InventoryWidgetClass)
    {
        GameMode->ShowPopupUI(InventoryWidgetClass);
    }
}

void UESCMenuWidget::OnOptionClicked()
{
    ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
    if (GameMode && OptionMenuWidgetClass)
    {
        GameMode->ShowPopupUI(OptionMenuWidgetClass);
    }
}

void UESCMenuWidget::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
