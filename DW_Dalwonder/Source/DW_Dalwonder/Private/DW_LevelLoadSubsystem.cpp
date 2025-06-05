// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_LevelLoadSubsystem.h"
#include "DW_GameInstance.h"
#include "DW_GmBase.h"
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

void UDW_LevelLoadSubsystem::StreamLevelAsync(FName LevelName)
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (LoadingWidgetClass)
    {
        LoadingWidget = CreateWidget<ULoadingWidget>(World, LoadingWidgetClass);
        if (LoadingWidget)
        {
            LoadingWidget->AddToViewport(1000);
            LoadingWidget->OnProgressUpdated(0.0f);
        }
    }

    CurrentProgress = 0.0f;

    bool bSuccess = false;

    //스트리밍으로 맵 열기
    StreamingLevel = ULevelStreamingDynamic::LoadLevelInstance(
        World,
        TEXT("/Game/ThirdPerson/Maps/Start_Testmap_WP"), // 나중에 맵 이름과 경로가 정형화되면 바꾸기
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        bSuccess
    );

    if (!StreamingLevel) return;

    //로딩바 타이머
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

    // 스트리밍이 끝났을 경우
    if (bLoaded && bVisible)
    {
        // 100% 강제 갱신
        if (LoadingWidget)
        {
            LoadingWidget->OnProgressUpdated(1.0f);
        }

        // 타이머 종료 및 스트리밍 핸들 제거
        StreamingLevel = nullptr;
        GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);

        // 캐릭터 스폰 시작 (위젯 제거는 이 안에서)
        WaitForPlayerStartThenSpawn();
        return;
    }

    // 진행률 99%까지만 증가
    CurrentProgress = FMath::Clamp(CurrentProgress + 0.05f, 0.f, 0.99f);
    LoadingWidget->OnProgressUpdated(CurrentProgress);
}

void UDW_LevelLoadSubsystem::SpawnPlayerCharacterAtPlayerStart()
{
    UWorld* World = GetWorld();
    if (!World) return;

    UDW_GameInstance* GameInstance = Cast<UDW_GameInstance>(UGameplayStatics::GetGameInstance(World));
    if (!GameInstance) return;

    // 기존 PlayerController 재사용
    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController를 찾을 수 없습니다."));
        return;
    }

    FVector SpawnLocation = FVector::ZeroVector;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    // 저장된 게임, 새 게임에 따라 로딩위치 다르게
    if (!GameInstance->bIsNewGame && GameInstance->LoadedSaveGame)
    {
        SpawnLocation = GameInstance->LoadedSaveGame->SavedPlayerLocation;
        SpawnRotation = GameInstance->LoadedSaveGame->SavedPlayerRotation;
    }
    else
    {
        // 실제 맵에 있는 PlayerStart 검색
        TArray<AActor*> PlayerStarts;
        UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), PlayerStarts);

        // 플레이어 스타트에서 스폰(PlayerStart가 로딩될때까지 기다리기때문에 else는 없음)
        if (PlayerStarts.Num() > 0)
        {
            AActor* Start = PlayerStarts[0];
            SpawnLocation = Start->GetActorLocation();
            SpawnRotation = Start->GetActorRotation();
        }
    }

    // GameMode에서 설정된 PawnClass 가져오기
    ADW_GmBase* GmBase = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(World));
    if (!GmBase || !*GmBase->DefaultPawnClass)
    {
        UE_LOG(LogTemp, Error, TEXT("GameMode 또는 DefaultPawnClass가 설정되지 않았습니다."));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = PC;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    //새로운 폰 스폰
    APawn* NewPawn = World->SpawnActor<APawn>(
        GmBase->DefaultPawnClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    // 폰 스폰 실패시
    if (!NewPawn) return;

    PC->Possess(NewPawn);

    // 저장 데이터 적용
    if (!GameInstance->bIsNewGame)
    {
        GameInstance->ApplyLoadedData();
        GameInstance->bIsNewGame = true;
    }

    // 로딩 위젯 제거
    if (LoadingWidget)
    {
        LoadingWidget->RemoveFromParent();
        LoadingWidget = nullptr;
    }
}

void UDW_LevelLoadSubsystem::WaitForPlayerStartThenSpawn()
{
    UWorld* World = GetWorld();
    if (!World) return;

    // PlayerStart가 나타날 때까지 주기적으로 체크
    World->GetTimerManager().SetTimerForNextTick([this]()
        {
            TArray<AActor*> PlayerStarts;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

            if (PlayerStarts.Num() > 0)
            {
                UE_LOG(LogTemp, Log, TEXT("PlayerStart 발견됨. 캐릭터 스폰 시작."));
                SpawnPlayerCharacterAtPlayerStart(); // 여기서 진짜 스폰
            }
            else
            {
                // 아직 없음 → 다시 대기
                GetWorld()->GetTimerManager().SetTimerForNextTick([this]() { WaitForPlayerStartThenSpawn(); });
            }
        });
}
