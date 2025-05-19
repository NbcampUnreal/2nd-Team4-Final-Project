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

    if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        // 반환값으로 방금 만든 위젯을 바로 받음
        if (UStarcatcher* StarUI = Cast<UStarcatcher>(GM->ShowPopupUI(StarcatcherClass)))
        {
            StarUI->OnStarCatcherFinished.AddDynamic(this, &UAlchemyUI::OnStarcatcherFinished);
            StarUI->SetFocus();   // 키 입력·스페이스 처리도 바로 활성
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
    case 3: NewPercentText = TEXT("성공 횟수 3 회"); break;
    case 2: NewPercentText = TEXT("성공 횟수 2 회"); break;
    case 1: NewPercentText = TEXT("성공 횟수 1 회"); break;
    default: NewPercentText = TEXT("성공 횟수 0 회"); break;
    }

    if (Percent)
    {
        Percent->SetText(FText::FromString(NewPercentText));
    }
}