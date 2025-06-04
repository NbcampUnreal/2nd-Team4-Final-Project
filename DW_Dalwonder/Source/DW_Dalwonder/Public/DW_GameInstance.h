#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DW_GameInstance.generated.h"

class UDW_SaveGame;
class UDW_LevelLoadSubsystem;
class ULoadingWidget;

/** GameInstance: OpenLevel + MoviePlayer �ε� ��ũ�� ���� */
UCLASS()
class DW_DALWONDER_API UDW_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;

    // 게임 저장
    UFUNCTION(BlueprintCallable, Category = "SaveSystem")
    void SaveGameData();

    //게임 불러오기
    UFUNCTION(BlueprintCallable, Category = "SaveSystem")
    void LoadGameData();

    UFUNCTION(BlueprintCallable)
    void ApplyLoadedData();  // 로드 완료

    /** 로딩 스크린을 보여주며 맵을 로드 */
    UFUNCTION(BlueprintCallable)
    void LoadLevelWithLoadingScreen(FName LevelName);


private:

    UPROPERTY()
    UDW_SaveGame* LoadedSaveGame = nullptr;

    FString DefaultSaveSlot = TEXT("DW_SaveData");

    // 로딩 위젯 클래스 (블루프린트에서 세팅)
    UPROPERTY(EditAnywhere, Category = "Loading")
    TSubclassOf<ULoadingWidget> LoadingWidgetClass;

    // 로딩 위젯 인스턴스
    //UPROPERTY()
    //TObjectPtr<ULoadingWidget> LoadingWidget;

    //// 로딩 Tick 타이머 핸들
    //FTimerHandle LoadingTickHandle;

    //// 진행률 콜백
    //void HandleProgressUpdated(float Progress);

    //// 완료 콜백
    //void HandleLoadingFinished();

    //// 로딩 진행 갱신
    //void TickLoading();

};