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
        // ��ȯ������ ��� ���� ������ �ٷ� ����
        if (UStarcatcher* StarUI = Cast<UStarcatcher>(GM->ShowPopupUI(StarcatcherClass)))
        {
            StarUI->OnStarCatcherFinished.AddDynamic(this, &UAlchemyUI::OnStarcatcherFinished);
            StarUI->SetFocus();   // Ű �Է¡������̽� ó���� �ٷ� Ȱ��
        }
    }
}

void UAlchemyUI::OnStarcatcherFinished(int32 SuccessCount)
{
    if (ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        // ���� �������� ��� �˾� �ݱ�
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