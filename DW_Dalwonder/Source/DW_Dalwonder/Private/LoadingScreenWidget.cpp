#include "LoadingScreenWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void ULoadingScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    TipList = {
        TEXT("TIP: 구르기는 무적 프레임이 있어요!"),
        TEXT("TIP: 적의 패턴을 관찰하세요."),
        TEXT("TIP: 강공격은 스태미너를 많이 소모합니다."),
        TEXT("TIP: 스킬트리는 초기화할 수 없어요!"),
        TEXT("TIP: 무기를 강화하면 큰 도움이 됩니다!")
    };

    // 첫 팁 설정
    if (Text_Tip)
    {
        Text_Tip->SetText(FText::FromString(TipList[0]));
    }

    // 타이머로 5초마다 변경
    GetWorld()->GetTimerManager().SetTimer(
        TipChangeTimer,
        this,
        &ULoadingScreenWidget::ChangeTipText,
        5.0f,
        true
    );
}

void ULoadingScreenWidget::NativeDestruct()
{
    // 위젯 종료 시 타이머 정리
    GetWorld()->GetTimerManager().ClearTimer(TipChangeTimer);
    Super::NativeDestruct();
}

void ULoadingScreenWidget::ChangeTipText()
{
    if (TipList.Num() == 0 || !Text_Tip) return;

    CurrentTipIndex = (CurrentTipIndex + 1) % TipList.Num();
    Text_Tip->SetText(FText::FromString(TipList[CurrentTipIndex]));
}

void ULoadingScreenWidget::SetLoadingPercent(float Percent)
{
    if (ProgressBar_Loading)
    {
        ProgressBar_Loading->SetPercent(Percent);
    }
}