#include "DW_Smite.h"
#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"

ADW_Smite::ADW_Smite()
{
    // 기본 생성자
}

void ADW_Smite::Interact_Implementation(AActor* Interactor)
{
    Super::Interact_Implementation(Interactor);

    //여기에 상호작용시 로직 작성
    if (!SmiteWidgetClass) return;

    if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
    {
        GM->ShowPopupUI(SmiteWidgetClass);
    }
}
