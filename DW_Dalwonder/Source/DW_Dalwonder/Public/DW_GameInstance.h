#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DW_GameInstance.generated.h"

class ULoadingScreenWidget;

/** GameInstance: OpenLevel + MoviePlayer 로딩 스크린 관리 */
UCLASS()
class DW_DALWONDER_API UDW_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    UFUNCTION(BlueprintCallable)
    void LoadLevelWithLoadingScreen();

    UFUNCTION(BlueprintCallable)
    void LoadLevelWithLoadingScreenByName(FName LevelName);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    FName LevelToLoad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading")
    TSubclassOf<ULoadingScreenWidget> LoadingWidgetClass;

    FTimerHandle DelayTest;

    void StartDelayTest();

    void EndDelayTest();

private:
    void ShowLoadingScreen();
    void HideLoadingScreen();

    void BeginLoadingScreen(const FString& MapName);
    void EndLoadingScreen(UWorld* LoadedWorld);

    UPROPERTY()
    class ULoadingScreenWidget* LoadingWidget;

};