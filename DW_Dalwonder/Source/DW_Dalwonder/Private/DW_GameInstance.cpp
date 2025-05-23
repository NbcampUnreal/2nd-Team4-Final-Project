#include "DW_GameInstance.h"
#include "LoadingScreenWidget.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"
#include "Engine/World.h"
#include "Misc/CoreDelegates.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void UDW_GameInstance::Init()
{
    Super::Init();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UDW_GameInstance::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UDW_GameInstance::EndLoadingScreen);
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