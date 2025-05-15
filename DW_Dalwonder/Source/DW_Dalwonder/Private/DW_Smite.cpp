#include "DW_Smite.h"
#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"

ADW_Smite::ADW_Smite()
{
    // �⺻ ������
}

void ADW_Smite::Interact_Implementation(AActor* Interactor)
{
    Super::Interact_Implementation(Interactor);

    //���⿡ ��ȣ�ۿ�� ���� �ۼ�
    if (!SmiteWidgetClass) return;

    if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
    {
        GM->ShowPopupUI(SmiteWidgetClass);
    }
}
