#include "DW_GameInstance.h"
#include "LoadingScreenWidget.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"
#include "Engine/World.h"
#include "Misc/CoreDelegates.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Item/ItemDataManager.h"
#include "DW_SaveGame.h"
#include "Character/DW_CharacterBase.h"

void UDW_GameInstance::Init()
{
    Super::Init();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UDW_GameInstance::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UDW_GameInstance::EndLoadingScreen);

}

void UDW_GameInstance::Shutdown()
{
    Super::Shutdown();
}


void UDW_GameInstance::LoadLevelWithLoadingScreen()
{
    if (LevelToLoad.IsNone())
    {
        UE_LOG(LogTemp, Warning, TEXT("LevelToLoad is not set!"));
        return;
    }

    ShowLoadingScreen();

    GetWorld()->GetTimerManager().SetTimerForNextTick(
        FTimerDelegate::CreateLambda([this]()
            {
                UGameplayStatics::OpenLevel(this, LevelToLoad);
            }));
}

void UDW_GameInstance::LoadLevelWithLoadingScreenByName(FName LevelName)
{
    LevelToLoad = LevelName;
    LoadLevelWithLoadingScreen();
}

void UDW_GameInstance::SaveGameData()
{
    UDW_SaveGame* SaveGameInstance = Cast<UDW_SaveGame>(
        UGameplayStatics::CreateSaveGameObject(UDW_SaveGame::StaticClass())
    );

    if (!SaveGameInstance) return;

    // 플레이어 캐릭터 가져오기
    ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (PlayerCharacter)
    {
        SaveGameInstance->SavedPlayerLocation = PlayerCharacter->GetActorLocation();

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("DW_SaveSlot"), 0);

        UE_LOG(LogTemp, Log, TEXT("게임 저장 완료: 위치 = %s"), *SaveGameInstance->SavedPlayerLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("DW_CharacterBase 캐릭터를 찾을 수 없습니다."));
    }
}

void UDW_GameInstance::SaveGameDataToSlot(const FString& SlotName)
{
    UDW_SaveGame* SaveGameInstance = Cast<UDW_SaveGame>(
        UGameplayStatics::CreateSaveGameObject(UDW_SaveGame::StaticClass())
    );

    if (!SaveGameInstance) return;

    if (ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        SaveGameInstance->SavedPlayerLocation = PlayerCharacter->GetActorLocation();

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

        UE_LOG(LogTemp, Log, TEXT("게임 저장 완료 [%s]"), *SlotName);
    }
}

void UDW_GameInstance::LoadGameData()
{
    // 저장된 게임이 존재하는지 확인
    if (!UGameplayStatics::DoesSaveGameExist(TEXT("DW_SaveSlot"), 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("세이브 슬롯이 존재하지 않습니다."));
        return;
    }

    // 세이브 파일 로드
    UDW_SaveGame* LoadedGame = Cast<UDW_SaveGame>(
        UGameplayStatics::LoadGameFromSlot(TEXT("DW_SaveSlot"), 0)
    );

    if (!LoadedGame)
    {
        UE_LOG(LogTemp, Warning, TEXT("세이브 데이터를 로드하지 못했습니다."));
        return;
    }

    // 플레이어 캐릭터에 위치 적용
    ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (PlayerCharacter)
    {
        PlayerCharacter->SetActorLocation(LoadedGame->SavedPlayerLocation);
        UE_LOG(LogTemp, Log, TEXT("게임 로드 완료: 위치 = %s"), *LoadedGame->SavedPlayerLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("플레이어 캐릭터를 찾을 수 없습니다."));
    }
}

void UDW_GameInstance::LoadGameDataFromSlot(const FString& SlotName)
{
    if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("세이브 슬롯 %s 이 존재하지 않습니다."), *SlotName);
        return;
    }

    LoadedSaveGame = Cast<UDW_SaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotName, 0)
    );

    if (!LoadedSaveGame)
    {
        UE_LOG(LogTemp, Warning, TEXT("세이브 데이터를 로드하지 못했습니다: %s"), *SlotName);
        return;
    }

    // 세이브 데이터 성공적으로 로드 → 게임 맵으로 이동
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestLoadingMap"));
}

void UDW_GameInstance::ApplyLoadedData()
{
    if (!LoadedSaveGame) return;

    ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (PlayerCharacter)
    {
        PlayerCharacter->SetActorLocation(LoadedSaveGame->SavedPlayerLocation);
        UE_LOG(LogTemp, Log, TEXT("저장된 위치로 이동: %s"), *LoadedSaveGame->SavedPlayerLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("캐릭터를 찾을 수 없습니다."));
    }

    // 위치 적용 후 포인터 초기화해도 좋음
    LoadedSaveGame = nullptr;
}

void UDW_GameInstance::ShowLoadingScreen()
{
    if (LoadingWidget) { return; } // 이미 있으면 무시

    if (!LoadingWidgetClass) { return; } // 설정 안 된 경우

    LoadingWidget = CreateWidget<ULoadingScreenWidget>(this, LoadingWidgetClass);
    if (!LoadingWidget) { return; }

    LoadingWidget->AddToViewport(100);
    LoadingWidget->SetVisibility(ESlateVisibility::Visible);
}

void UDW_GameInstance::HideLoadingScreen()
{
    if (!LoadingWidget) { return; }

    LoadingWidget->RemoveFromParent();
    LoadingWidget = nullptr;
}


void UDW_GameInstance::BeginLoadingScreen(const FString& /*MapName*/)
{
    // UMG 위젯이 없다면 생성 (콘솔에서 "open Map" 등으로 갈 때도 대비)
    if (!LoadingWidget) { ShowLoadingScreen(); }

    if (!LoadingWidget) { return; }

    // UMG → Slate 변환
    TSharedPtr<SWidget> LoadingSlate = LoadingWidget->TakeWidget();

    FLoadingScreenAttributes Attr;
    Attr.bAutoCompleteWhenLoadingCompletes = false;         // 레벨 완료 후 수동 제거
    Attr.MinimumLoadingScreenDisplayTime = 10.0f;           // 최소 2초 노출
    Attr.WidgetLoadingScreen = LoadingSlate;

    GetMoviePlayer()->SetupLoadingScreen(Attr);
}

void UDW_GameInstance::EndLoadingScreen(UWorld* /*LoadedWorld*/)
{
    GetWorld()->GetTimerManager().SetTimer(DelayTest, this, &UDW_GameInstance::HideLoadingScreen, 3.0f, false);
}