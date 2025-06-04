// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_LevelLoadSubsystem.h"
#include "UI/Widget/LoadingWidget.h"
#include "Engine/LevelStreamingDynamic.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UDW_LevelLoadSubsystem::SetLoadingWidgetClass(TSubclassOf<ULoadingWidget> InClass)
{
    LoadingWidgetClass = InClass;
}

//void UDW_LevelLoadSubsystem::StartLoadingLevel(FName LevelName)
//{
//    PendingLevelName = LevelName.ToString();
//
//    FStringAssetReference LevelPath = FString::Printf(
//        TEXT("/Game/ThirdPerson/Maps/%s.%s"),
//        *PendingLevelName, *PendingLevelName
//    );
//
//    LoadingHandle = StreamableManager.RequestAsyncLoad(
//        LevelPath, FStreamableDelegate::CreateUObject(this, &UDW_LevelLoadSubsystem::OnLoadComplete)
//    );
//
//    if (!LoadingHandle.IsValid())
//    {
//        UE_LOG(LogTemp, Error, TEXT(" 로딩 핸들 생성 실패: %s"), *PendingLevelName);
//    }
//    else
//    {
//        UE_LOG(LogTemp, Log, TEXT(" 비동기 로딩 시작: %s"), *PendingLevelName);
//    }
//}
//
//void UDW_LevelLoadSubsystem::InternalTick()
//{
//    if (LoadingHandle.IsValid())
//    {
//        float Progress = LoadingHandle->GetProgress();
//        UE_LOG(LogTemp, Log, TEXT(" 진행률: %f"), Progress);
//        OnProgressUpdated.Broadcast(Progress);
//
//        if (LoadingHandle->HasLoadCompleted())
//        {
//            OnLoadingFinished.Broadcast();
//        }
//    }
//}
//
//void UDW_LevelLoadSubsystem::OnLoadComplete()
//{
//    UE_LOG(LogTemp, Log, TEXT(" 레벨 로딩 완료: %s"), *PendingLevelName);
//}
//
//FString UDW_LevelLoadSubsystem::GetPendingLevelName() const
//{
//    return PendingLevelName;
//}

void UDW_LevelLoadSubsystem::StreamLevelAsync(FName LevelName)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT(" StreamLevelAsync: World is null"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT(" 스트리밍 레벨 로딩 시작: %s"), *LevelName.ToString());

    // 로딩 위젯 생성
    if (LoadingWidgetClass)
    {
        LoadingWidget = CreateWidget<ULoadingWidget>(World, LoadingWidgetClass);
        if (LoadingWidget)
        {
            LoadingWidget->AddToViewport();
            LoadingWidget->OnProgressUpdated(0.0f);
        }
    }

    // 진행률 초기화
    CurrentProgress = 0.0f;

    // LatentInfo는 안 써도 되지만 형식상 작성
    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this;

    bool bSuccess = false;
    
    // 5.4에서 지원되는 스트리밍 함수 사용
    StreamingLevel = ULevelStreamingDynamic::LoadLevelInstance(
        GetWorld(),
        TEXT("/Game/ThirdPerson/Maps/Start_Testmap_WP"), // LevelName 나중에 바꾸기
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        bSuccess
    );

    if (!StreamingLevel)
    {
        UE_LOG(LogTemp, Error, TEXT(" 스트리밍 레벨 로딩 실패: %s"), *LevelName.ToString());
        return;
    }

    // 진행률 추적 시작
    World->GetTimerManager().SetTimer(
        TickTimerHandle,
        this,
        &UDW_LevelLoadSubsystem::TickStreamingProgress,
        0.1f,
        true
    );
}

void UDW_LevelLoadSubsystem::TickStreamingProgress()
{
    if (!StreamingLevel || !LoadingWidget) return;

    bool bLoaded = StreamingLevel->IsLevelLoaded();
    bool bVisible = StreamingLevel->IsLevelVisible();

    if (bLoaded && bVisible)
    {
        LoadingWidget->OnProgressUpdated(1.0f);
        UE_LOG(LogTemp, Log, TEXT(" 스트리밍 레벨 로딩 완료"));

        LoadingWidget->RemoveFromParent();
        LoadingWidget = nullptr;

        StreamingLevel = nullptr;
        GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
        return;
    }

    // 서서히 퍼센트 상승 (체감용 가짜 퍼센트)
    CurrentProgress = FMath::Clamp(CurrentProgress + 0.05f, 0.f, 0.95f);
    LoadingWidget->OnProgressUpdated(CurrentProgress);

    UE_LOG(LogTemp, Log, TEXT(" 로딩 진행률: %.2f%%"), CurrentProgress * 100);
}
