#pragma once

#include "CoreMinimal.h"
#include "DW_Portal.h"
#include "Engine/GameInstance.h"
#include "TmpSaveData.h"
#include "UI/Component/QuestDatabase.h"
#include "DW_GameInstance.generated.h"

class UDW_SaveGame;
class UDW_LevelLoadSubsystem;
class ULoadingWidget;
class ADW_CharacterBase;

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

    // 맵이동용 임시저장 데이터 저장
    void CacheTempDataBeforeLevelChange();
    // 맵 이동용 임시저장 데이터 캐릭터에 다시 부여
    void ApplyTempDataAfterLevelLoad();
    // 위치정보는 부여안하는 함수
    void ApplyTempDataAfterLevelLoadWithLocation();
    // 캐릭터 Component들 복원 함수
	void ApplyTempDatatoCharacterComponents(ADW_CharacterBase* Player);

public:

    UPROPERTY()
    USettingsManager* SettingsManager;

    UFUNCTION(BlueprintCallable)
    USettingsManager* GetSettingsManager() const { return SettingsManager; }

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
    EPortalType LastPortalType = EPortalType::None;

private:

    FString DefaultSaveSlot = TEXT("DW_SaveData");

    // 로딩 위젯 클래스 (블루프린트에서 세팅)
    UPROPERTY(EditAnywhere, Category = "Loading")
    TSubclassOf<ULoadingWidget> LoadingWidgetClass;

    // 맵 이동용 임시저장데이터
    UPROPERTY()
    FTmpSaveData CachedTempSaveData;

protected:
    UPROPERTY()
    class UItemDataManager* ItemDataManager; // 매니저 인스턴스 참조

    UPROPERTY(EditAnywhere, Category = "Data Tables")
    TSoftObjectPtr<UDataTable> ItemBaseDataTableRef; // 에디터에서 할당할 데이터 테이블 경로

};