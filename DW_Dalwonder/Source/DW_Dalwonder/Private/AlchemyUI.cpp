#include "AlchemyUI.h"
#include "Starcatcher.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "DW_GmBase.h"

void UAlchemyUI::NativeConstruct()
{
    Super::NativeConstruct();

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UAlchemyUI::OnStartButtonClicked);
    }
}

void UAlchemyUI::OnStartButtonClicked()
{
    if (!StarcatcherClass) return;

    // 팝업 UI 열기 (생성은 GameMode 내부에서)
    if (ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        GameMode->ShowPopupUI(StarcatcherClass);

        // 팝업 리스트 중 마지막(방금 띄운 것)을 가져와서 바인딩
        if (GameMode->PopupWidgets.Num() > 0)
        {
            if (UStarcatcher* StarcatcherUI = Cast<UStarcatcher>(GameMode->PopupWidgets.Last()))
            {
                StarcatcherUI->OnStarCatcherFinished.AddDynamic(this, &UAlchemyUI::OnStarcatcherFinished);
            }
        }
    }
}

void UAlchemyUI::OnStarcatcherFinished(int32 SuccessCount)
{
    if (ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        // 가장 마지막에 띄운 팝업 닫기
        GameMode->CloseLastPopupUI();
    }

    FString NewPercentText;
    switch (SuccessCount)
    {
    case 3: NewPercentText = TEXT("15%"); break;
    case 2: NewPercentText = TEXT("10%"); break;
    case 1: NewPercentText = TEXT("5%"); break;
    default: NewPercentText = TEXT("0%"); break;
    }

    if (Percent)
    {
        Percent->SetText(FText::FromString(NewPercentText));
    }
}