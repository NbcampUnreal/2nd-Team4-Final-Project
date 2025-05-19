#include "Starcatcher.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "StarEffectActor.h"

void UStarcatcher::NativeConstruct()
{
    Super::NativeConstruct();
    SetIsFocusable(true);
    CurrentStarX = 0.f;
    bGoingRight = true;
    SuccessCount = 0;
    CurrentTry = 0;
    bIsGameFinished = false;
    bZoneReady = false;
}

void UStarcatcher::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 성공 존 초기화
    if (!bZoneReady && StarCanvas && SuccessZoneImage)
    {
        InitSuccessZone();
    }

    if (!StarCanvas || !StarImage || bIsGameFinished) return;

    const float CanvasWidth = StarCanvas->GetCachedGeometry().GetLocalSize().X;
    const float MoveDistance = StarMoveSpeed * InDeltaTime;

    CurrentStarX += (bGoingRight ? MoveDistance : -MoveDistance);
    if (CurrentStarX >= CanvasWidth) { CurrentStarX = CanvasWidth; bGoingRight = false; }
    if (CurrentStarX <= 0.f) { CurrentStarX = 0.f; bGoingRight = true; }

    if (UCanvasPanelSlot* StarSlot = Cast<UCanvasPanelSlot>(StarImage->Slot))
    {
        StarSlot->SetPosition({ CurrentStarX, StarSlot->GetPosition().Y });
    }
}

void UStarcatcher::InitSuccessZone()
{
    const FVector2D CanvasSize = StarCanvas->GetCachedGeometry().GetLocalSize();
    if (CanvasSize.X <= 0.f) return;

    if (!SuccessZoneImage->GetParent())
    {
        StarCanvas->AddChild(SuccessZoneImage);
    }

    if (UCanvasPanelSlot* ZoneSlot = Cast<UCanvasPanelSlot>(SuccessZoneImage->Slot))
    {
        const float MaxX = CanvasSize.X - SuccessZoneWidth;
        const float RandX = FMath::FRandRange(0.f, FMath::Max(0.f, MaxX));

        ZoneSlot->SetAutoSize(false);
        ZoneSlot->SetSize({ SuccessZoneWidth, CanvasSize.Y });
        ZoneSlot->SetPosition({ RandX, 0.f });

        SuccessLeft = RandX;
        SuccessRight = RandX + SuccessZoneWidth;
        bZoneReady = true;
    }
}

void UStarcatcher::FinishGame()
{
    bIsGameFinished = true;
    FinalSuccessCount = SuccessCount;

    // 성공 횟수 로그 출력
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow,
            FString::Printf(TEXT("Final Success Count: %d"), FinalSuccessCount));
    }

    OnStarCatcherFinished.Broadcast(SuccessCount);
}

FReply UStarcatcher::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::SpaceBar)
    {
        HandleInput();
        return FReply::Handled();
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}


void UStarcatcher::HandleInput()
{
    if (!bZoneReady || bIsGameFinished || !StarImage) return;

    const float StarWidth = StarImage->GetDesiredSize().X;
    const float StarCenterX = CurrentStarX + (StarWidth * 0.5f);

    CurrentTry++;

    /* ───── 성공 판정 ───── */
    if (StarCenterX >= SuccessLeft && StarCenterX <= SuccessRight)
    {
        SuccessCount++;
        if (SuccessSound)
        {
            UGameplayStatics::PlaySound2D(this, SuccessSound);
        }
        // 이펙트
        if (SuccessEffects.IsValidIndex(SuccessCount - 1))
        {
            if (AActor* Owner = GetOwningPlayerPawn())
            {
                FVector SpawnLoc = Owner->GetActorLocation();
                FRotator SpawnRot = FRotator::ZeroRotator;

                AStarEffectActor* EffectActor = GetWorld()->SpawnActor<AStarEffectActor>(
                    AStarEffectActor::StaticClass(), SpawnLoc, SpawnRot
                );

                if (EffectActor)
                {
                    EffectActor->InitEffect(SuccessEffects[SuccessCount - 1], SpawnLoc, SpawnRot);
                }
            }
        }

        StarImage->SetVisibility(ESlateVisibility::Hidden);

        EndRound(/*bDelayRestart=*/true);
        return;
    }

    if (CurrentTry >= MaxTry)
    {
        if (FailSound)
        {
            UGameplayStatics::PlaySound2D(this, FailSound);
        }
        if (FailEffect)
        {
            if (AActor* Owner = GetOwningPlayerPawn())
            {
                FVector SpawnLoc = Owner->GetActorLocation();
                FRotator SpawnRot = FRotator::ZeroRotator;

                if (AStarEffectActor* FXActor = GetWorld()->SpawnActor<AStarEffectActor>(
                    AStarEffectActor::StaticClass(), SpawnLoc, SpawnRot))
                {
                    FXActor->InitEffect(FailEffect, SpawnLoc, SpawnRot);
                }
            }
        }
        EndRound(/*bDelayRestart=*/false);
    }
}

void UStarcatcher::RestartGame()
{
    GetWorld()->GetTimerManager().ClearTimer(RestartTimerHandle);

    CurrentTry = 0;
    CurrentStarX = 0.f;
    bGoingRight = true;
    bZoneReady = false;

    if (StarImage)
        StarImage->SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogTemp, Warning, TEXT("게임 재시작! 다음 라운드 번호 = %d"), CurrentGame + 1);
}

void UStarcatcher::EndRound(bool bDelayRestart)
{
    GetWorld()->GetTimerManager().ClearTimer(RestartTimerHandle);

    ++CurrentGame;
    UE_LOG(LogTemp, Warning, TEXT("라운드 종료 ➜ 현재 게임 번호: %d"), CurrentGame);

    if (CurrentGame >= MaxGame)
    {
        FinishGame();
        return;
    }

    if (bDelayRestart)
    {
        GetWorld()->GetTimerManager().SetTimer(
            RestartTimerHandle, this, &UStarcatcher::RestartGame, 1.f, false);
    }
    else
    {
        RestartGame();
    }
}