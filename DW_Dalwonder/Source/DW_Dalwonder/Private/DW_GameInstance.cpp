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

    ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerCharacter) return;

    SaveGameInstance->SavedPlayerLocation = PlayerCharacter->GetActorLocation();

    if (UDW_AttributeComponent* AttrComp = PlayerCharacter->FindComponentByClass<UDW_AttributeComponent>())
    {
        AttrComp->SaveData(SaveGameInstance->SavedAttributes);
    }

    UGameplayStatics::SaveGameToSlot(SaveGameInstance, DefaultSaveSlot, 0);
}

void UDW_GameInstance::LoadGameData()
{
    if (!UGameplayStatics::DoesSaveGameExist(DefaultSaveSlot, 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("기본 저장 슬롯 [%s] 이 존재하지 않습니다."), *DefaultSaveSlot);
        return;
    }

    LoadedSaveGame = Cast<UDW_SaveGame>(
        UGameplayStatics::LoadGameFromSlot(DefaultSaveSlot, 0)
    );

    if (!LoadedSaveGame)
    {
        UE_LOG(LogTemp, Warning, TEXT("저장 데이터를 불러오지 못했습니다."));
        return;
    }

    UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestLoadingMap"));
}

void UDW_GameInstance::ApplyLoadedData()
{
    if (!LoadedSaveGame) return;

    ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerCharacter) return;

    // 위치 적용
    PlayerCharacter->SetActorLocation(LoadedSaveGame->SavedPlayerLocation);

    // Attribute 적용
    if (UDW_AttributeComponent* AttrComp = PlayerCharacter->FindComponentByClass<UDW_AttributeComponent>())
    {
        AttrComp->LoadData(LoadedSaveGame->SavedAttributes);
    }

    UE_LOG(LogTemp, Log, TEXT("게임 불러오기 완료: 위치 적용 및 스탯 복원"));

    LoadedSaveGame = nullptr; // 일회성 데이터로 초기화
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