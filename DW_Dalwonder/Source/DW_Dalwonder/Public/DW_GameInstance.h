#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DW_GameInstance.generated.h"

class ULoadingScreenWidget;
class UDW_SaveGame;

/** GameInstance: OpenLevel + MoviePlayer �ε� ��ũ�� ���� */
UCLASS()
class DW_DALWONDER_API UDW_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;

    UFUNCTION(BlueprintCallable)
    void LoadLevelWithLoadingScreen();

    UFUNCTION(BlueprintCallable)
    void LoadLevelWithLoadingScreenByName(FName LevelName);

    // 게임 저장
    UFUNCTION(BlueprintCallable, Category = "SaveSystem")
    void SaveGameData();

    //게임 불러오기
    UFUNCTION(BlueprintCallable, Category = "SaveSystem")
    void LoadGameData();

    UFUNCTION(BlueprintCallable)
    void ApplyLoadedData();  // 로드 완료

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    FName LevelToLoad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading")
    TSubclassOf<ULoadingScreenWidget> LoadingWidgetClass;

protected:
    

public:

    FTimerHandle DelayTest;

    void StartDelayTest();

    void EndDelayTest();

private:
    void ShowLoadingScreen();
    void HideLoadingScreen();

    void BeginLoadingScreen(const FString& MapName);
    void EndLoadingScreen(UWorld* LoadedWorld);

private:

    UPROPERTY()
    class ULoadingScreenWidget* LoadingWidget;

    FString DefaultSaveSlot = TEXT("DW_SaveData");
    UDW_SaveGame* LoadedSaveGame = nullptr;

};