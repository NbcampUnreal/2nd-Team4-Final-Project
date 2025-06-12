#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/Component/QuestDatabase.h"
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

    /** 로딩맵에서 호출할 함수 */
    UFUNCTION(BlueprintCallable)
    void StartLevelStreaming();

public:

    UPROPERTY()
    UDW_SaveGame* LoadedSaveGame = nullptr;

    UPROPERTY(BlueprintReadWrite, Category = "Game Start Type")
    bool bIsNewGame = true; // 기본값은 새 게임

    UPROPERTY()
    UDW_LevelLoadSubsystem* LevelLoadSubsystem;

    UPROPERTY()
    FName PendingLevelName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
    UQuestDatabase* QuestDatabase;

private:

    FString DefaultSaveSlot = TEXT("DW_SaveData");

    // 로딩 위젯 클래스 (블루프린트에서 세팅)
    UPROPERTY(EditAnywhere, Category = "Loading")
    TSubclassOf<ULoadingWidget> LoadingWidgetClass;

};