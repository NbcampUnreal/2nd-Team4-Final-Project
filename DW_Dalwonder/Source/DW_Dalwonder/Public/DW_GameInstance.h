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

    UFUNCTION(BlueprintCallable)
    void SaveGameDataToSlot(const FString& SlotName);

    //게임 불러오기
    UPROPERTY()
    UDW_SaveGame* LoadedSaveGame;  // 로드된 세이브 데이터를 임시 저장

    UFUNCTION(BlueprintCallable, Category = "SaveSystem")
    void LoadGameData();

    UFUNCTION(BlueprintCallable)
    void LoadGameDataFromSlot(const FString& SlotName);

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

    UPROPERTY()
    class ULoadingScreenWidget* LoadingWidget;

};