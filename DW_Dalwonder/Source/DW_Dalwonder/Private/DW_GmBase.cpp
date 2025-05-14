#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

ADW_GmBase::ADW_GmBase()
{
    CurrentWidget = nullptr;
}

void ADW_GmBase::BeginPlay()
{
    Super::BeginPlay();

}

void ADW_GmBase::SwitchUI(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (NewWidgetClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void ADW_GmBase::ShowPopupUI(TSubclassOf<UUserWidget> WidgetClass)
{
    if (!WidgetClass) return;

    UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
    if (NewWidget)
    {
        NewWidget->AddToViewport(10);
        PopupWidgets.Add(NewWidget);

        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            PC->bShowMouseCursor = true;

            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(NewWidget->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            InputMode.SetHideCursorDuringCapture(false);
            PC->SetInputMode(InputMode);
        }
    }
}


void ADW_GmBase::ClosePopupUI(UUserWidget* WidgetToRemove)
{
    if (!WidgetToRemove) return;

    WidgetToRemove->RemoveFromParent();
    PopupWidgets.Remove(WidgetToRemove);

    if (PopupWidgets.Num() == 0)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            PC->bShowMouseCursor = false;
            FInputModeGameOnly InputMode;
            PC->SetInputMode(InputMode);
        }
    }
}

void ADW_GmBase::CloseLastPopupUI()
{
    if (PopupWidgets.Num() == 0)
        return;

    UUserWidget* LastWidget = PopupWidgets.Last();
    if (LastWidget)
    {
        LastWidget->RemoveFromParent();
        PopupWidgets.Pop();
    }

    if (PopupWidgets.Num() == 0)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            PC->bShowMouseCursor = false;

            FInputModeGameAndUI InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
        }
    }
}


//void ADW_PlayerController::SetupInputComponent()
//{
//    Super::SetupInputComponent();
//
//    InputComponent->BindAction("ClosePopup", IE_Pressed, this, &AMyPlayerController::HandleClosePopup);
//}
//
//void ADW_PlayerController::HandleClosePopup()
//{
//    if (AMyGameModeBase* GM = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this)))
//    {
//        GM->CloseLastPopupUI();
//    }
//}