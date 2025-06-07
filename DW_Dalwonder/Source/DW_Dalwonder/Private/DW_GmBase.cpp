#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "DW_GameInstance.h"
#include "UI/Widget/ResultWidget.h"
#include "DW_SaveGame.h"

ADW_GmBase::ADW_GmBase()
{
    CurrentWidget = nullptr;
    // 자동 Pawn 스폰 막기
    bStartPlayersAsSpectators = true;
}

void ADW_GmBase::BeginPlay()
{
    Super::BeginPlay();

    if (UDW_GameInstance* GI = Cast<UDW_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
    {
        GI->ApplyLoadedData();
    }
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

UUserWidget* ADW_GmBase::ShowPopupUI(TSubclassOf<UUserWidget> WidgetClass)
{
    if (!WidgetClass) return nullptr;

    // 위젯 생성
    UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
    if (!NewWidget) return nullptr;

    // 화면에 추가
    NewWidget->AddToViewport(10);
    PopupWidgets.Add(NewWidget);

    // 입력‧마우스 세팅
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PC->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(NewWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);
        PC->SetInputMode(InputMode);
    }

    return NewWidget;   // 반환
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

UUserWidget* ADW_GmBase::CloseLastPopupUI_AndReturn()
{
    if (PopupWidgets.Num() == 0)
        return nullptr;

    UUserWidget* LastWidget = PopupWidgets.Last();
    if (LastWidget)
    {
        LastWidget->RemoveFromParent();
        PopupWidgets.Pop();

        // 마우스 커서와 InputMode도 필요한 경우 여기서 처리
        if (PopupWidgets.Num() == 0)
        {
            if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
            {
                PC->SetShowMouseCursor(false);
                PC->SetInputMode(FInputModeGameOnly());
            }
        }
    }

    return LastWidget;
}

void ADW_GmBase::ShowResultUI(const FString& MessageText)
{
    if (!ResultWidgetClass) return;

    UE_LOG(LogTemp, Warning, TEXT("dd"));

    UResultWidget* ResultUI = Cast<UResultWidget>(ShowPopupUI(ResultWidgetClass));
    if (ResultUI)
    {
        ResultUI->SetResultText(MessageText);
        ResultUI->StartLetterSpacingAnimation();

        if (ResultUI->ResultTextAnimation)
        {
            ResultUI->PlayAnimation(ResultUI->ResultTextAnimation);
        }

        FTimerHandle Handle;
        GetWorld()->GetTimerManager().SetTimer(Handle, [=, this]()
        {
            this->ClosePopupUI(ResultUI);
        }, 5.f, false);
    }
}
