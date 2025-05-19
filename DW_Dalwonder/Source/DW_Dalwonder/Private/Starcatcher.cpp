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

    // 별 위치 갱신
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

    // 성공 판정
    if (CurrentStarX >= Left && CurrentStarX <= Right)
    {
        SuccessCount++;

        // 이펙트 출력
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

    // 3번 시도 후 게임 종료
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
    /* SpaceBar 눌렀다면 HandleInput() 실행 */
    if (InKeyEvent.GetKey() == EKeys::SpaceBar)
    {
        HandleInput();
        return FReply::Handled();   // ← 더 이상 아래로 키 이벤트 전달 안 함
    }

    /* 그 외 키는 기본 처리 */
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}