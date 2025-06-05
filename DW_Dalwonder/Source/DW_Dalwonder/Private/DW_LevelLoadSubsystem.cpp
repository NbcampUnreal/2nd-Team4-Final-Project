// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_LevelLoadSubsystem.h"
#include "DW_GameInstance.h"
#include "DW_SaveGame.h"
#include "UI/Widget/LoadingWidget.h"
#include "Character/DW_CharacterBase.h"
#include "Character/DW_PlayerController.h"
#include "Engine/LevelStreamingDynamic.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"

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
        UE_LOG(LogTemp, Error, TEXT("StreamLevelAsync: World is null"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("스트리밍 레벨 로딩 시작: %s"), *LevelName.ToString());

    if (LoadingWidgetClass)
    {
        LoadingWidget = CreateWidget<ULoadingWidget>(World, LoadingWidgetClass);
        if (LoadingWidget)
        {
            LoadingWidget->AddToViewport();
            LoadingWidget->OnProgressUpdated(0.0f);
        }
    }

    CurrentProgress = 0.0f;

    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this;

    bool bSuccess = false;

    StreamingLevel = ULevelStreamingDynamic::LoadLevelInstance(
        World,
        TEXT("/Game/ThirdPerson/Maps/Start_Testmap_WP"), // 나중에 바뀔 수 있음
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        bSuccess
    );

    if (!StreamingLevel)
    {
        UE_LOG(LogTemp, Error, TEXT("스트리밍 레벨 로딩 실패: %s"), *LevelName.ToString());
        return;
    }

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
        UE_LOG(LogTemp, Log, TEXT("스트리밍 레벨 로딩 완료"));

        LoadingWidget->RemoveFromParent();
        LoadingWidget = nullptr;

        StreamingLevel = nullptr;
        GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);

        //  캐릭터 수동 스폰 호출
        SpawnPlayerCharacterAtPlayerStart();

        return;
    }

    CurrentProgress = FMath::Clamp(CurrentProgress + 0.05f, 0.f, 0.95f);
    LoadingWidget->OnProgressUpdated(CurrentProgress);

    UE_LOG(LogTemp, Log, TEXT("로딩 진행률: %.2f%%"), CurrentProgress * 100);
}

void UDW_LevelLoadSubsystem::SpawnPlayerCharacterAtPlayerStart()
{
    UWorld* World = GetWorld();
    if (!World) return;

    UDW_GameInstance* GameInstance = Cast<UDW_GameInstance>(UGameplayStatics::GetGameInstance(World));
    if (!GameInstance) return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC) return;

    FVector SpawnLocation;
    FRotator SpawnRotation;

    if (!GameInstance->bIsNewGame && GameInstance->LoadedSaveGame)
    {
        // 저장된 위치에서 스폰
        SpawnLocation = GameInstance->LoadedSaveGame->SavedPlayerLocation;
        SpawnRotation = GameInstance->LoadedSaveGame->SavedPlayerRotation;
    }
    else
    {
        // PlayerStart 위치
        TArray<AActor*> PlayerStarts;
        UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), PlayerStarts);

        if (PlayerStarts.Num() == 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Spawn 실패: PlayerStart 없음"));
            return;
        }

        AActor* StartPoint = PlayerStarts[0];
        SpawnLocation = StartPoint->GetActorLocation();
        SpawnRotation = StartPoint->GetActorRotation();
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = PC;

    ADW_CharacterBase* NewCharacter = World->SpawnActor<ADW_CharacterBase>(ADW_CharacterBase::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
    if (NewCharacter)
    {
        PC->Possess(NewCharacter);
        UE_LOG(LogTemp, Log, TEXT("플레이어 수동 스폰 완료"));

        // 저장된 데이터 적용
        if (!GameInstance->bIsNewGame)
        {
            GameInstance->ApplyLoadedData();
            GameInstance->bIsNewGame = true;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("플레이어 스폰 실패"));
    }
}
