#include "DW_NpcBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ADW_NpcBase::ADW_NpcBase()
{
}

void ADW_NpcBase::Interact_Implementation(AActor* Interactor)
{
    Super::Interact_Implementation(Interactor);

    if (DialogueUIClass && Interactor)
    {
        APlayerController* PC = Cast<APlayerController>(Interactor->GetInstigatorController());
        if (PC)
        {
            // UI 구성 요소 추가
            DialogueUIInstance = CreateWidget<UUserWidget>(PC, DialogueUIClass);
            if (DialogueUIInstance)
            {
                DialogueUIInstance->AddToViewport();
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());

                // 카메라 전환
                FocusCameraOnNPC(Interactor);
            }
        }
    }
}

void ADW_NpcBase::FocusCameraOnNPC(AActor* PlayerActor)
{
    if (!NPCInteractionCamera) return;

    APlayerController* PC = Cast<APlayerController>(PlayerActor->GetInstigatorController());
    if (PC)
    {
        // 새 타입 등록 (사용자 카메라 전환)
        PC->SetViewTargetWithBlend(NPCInteractionCamera, 1.0f); // 1차 테스트
    }
}


