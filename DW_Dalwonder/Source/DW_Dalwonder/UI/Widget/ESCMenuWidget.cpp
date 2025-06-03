// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ESCMenuWidget.h"
#include "UI/Widget/CustomButtonWidget.h"
#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DW_GameInstance.h"
#include "UI/Widget/SavedGameWidget.h"

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

    if (SaveButton)
        SaveButton->OnCustomClicked.AddDynamic(this, &UESCMenuWidget::OnSaveClicked);

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

void UESCMenuWidget::OnSaveClicked()
{
    if (ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
    {
        if (SavedGameWidgetClass)
        {
            // ShowPopupUI는 생성된 UUserWidget*을 반환함
            UUserWidget* CreatedWidget = GameMode->ShowPopupUI(SavedGameWidgetClass);

            // SavedGameWidget 타입으로 캐스팅 후 모드 설정
            if (USavedGameWidget* SaveWidget = Cast<USavedGameWidget>(CreatedWidget))
            {
                SaveWidget->SetMode(ESaveGameWidgetMode::Save);
            }
        }
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
