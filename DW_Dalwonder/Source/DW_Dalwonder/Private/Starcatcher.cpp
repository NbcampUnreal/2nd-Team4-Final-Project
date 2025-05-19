#include "Starcatcher.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"


void UStarcatcher::NativeConstruct()
{
    Super::NativeConstruct();
    SetIsFocusable(true);
    CurrentStarX = 0.f;
    bGoingRight = true;
    SuccessCount = 0;
    CurrentTry = 0;
    bIsGameFinished = false;
}

void UStarcatcher::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!StarCanvas || !StarImage || bIsGameFinished)
        return;

    float CanvasWidth = StarCanvas->GetCachedGeometry().GetLocalSize().X;
    float MoveDistance = StarMoveSpeed * InDeltaTime;

    if (bGoingRight)
    {
        CurrentStarX += MoveDistance;
        if (CurrentStarX >= CanvasWidth)
        {
            CurrentStarX = CanvasWidth;
            bGoingRight = false;
        }
    }
    else
    {
        CurrentStarX -= MoveDistance;
        if (CurrentStarX <= 0.f)
        {
            CurrentStarX = 0.f;
            bGoingRight = true;
        }
    }

    // �� ��ġ ����
    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(StarImage->Slot))
    {
        CanvasSlot->SetPosition(FVector2D(CurrentStarX, CanvasSlot->GetPosition().Y));
    }
}

void UStarcatcher::HandleInput()
{
    if (bIsGameFinished || !StarCanvas)
        return;

    float CanvasWidth = StarCanvas->GetCachedGeometry().GetLocalSize().X;
    float TargetX = CanvasWidth * SuccessZoneRatio;
    float Left = TargetX - (SuccessZoneWidth * 0.5f);
    float Right = TargetX + (SuccessZoneWidth * 0.5f);

    CurrentTry++;

    // ���� ����
    if (CurrentStarX >= Left && CurrentStarX <= Right)
    {
        SuccessCount++;

        // ����Ʈ ���
        if (SuccessEffects.IsValidIndex(SuccessCount - 1))
        {
            AActor* Owner = GetOwningPlayerPawn();
            if (Owner && SuccessEffects.IsValidIndex(SuccessCount - 1) && SuccessEffects[SuccessCount - 1])
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    GetWorld(),
                    SuccessEffects[SuccessCount - 1],
                    Owner->GetActorLocation(),
                    FRotator::ZeroRotator,
                    FVector(1.f),
                    true,
                    true
                );
            }
        }
    }

    // 3�� �õ� �� ���� ����
    if (CurrentTry >= MaxTry)
    {
        FinishGame();
    }
}

void UStarcatcher::FinishGame()
{
    bIsGameFinished = true;
    OnStarCatcherFinished.Broadcast(SuccessCount);
}

FReply UStarcatcher::NativeOnKeyDown(const FGeometry& InGeometry,
    const FKeyEvent& InKeyEvent)
{
    /* SpaceBar �����ٸ� HandleInput() ���� */
    if (InKeyEvent.GetKey() == EKeys::SpaceBar)
    {
        HandleInput();
        return FReply::Handled();   // �� �� �̻� �Ʒ��� Ű �̺�Ʈ ���� �� ��
    }

    /* �� �� Ű�� �⺻ ó�� */
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}