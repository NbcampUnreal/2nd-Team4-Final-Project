#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DW_GameInstance.generated.h"

class ULoadingScreenWidget;

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

protected:
    UPROPERTY()
    class UItemDataManager* ItemDataManager; // 매니저 인스턴스 참조

    UPROPERTY(EditAnywhere, Category = "Data Tables")
    TSoftObjectPtr<UDataTable> ItemBaseDataTableRef; // 에디터에서 할당할 데이터 테이블 경로

};