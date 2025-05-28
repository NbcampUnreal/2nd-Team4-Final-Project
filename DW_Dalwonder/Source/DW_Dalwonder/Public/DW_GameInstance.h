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
    
    UPROPERTY() // 가비지 컬렉션의 관리를 받도록 UPROPERTY 추가
    class UItemDataManager* ItemDataManagerInstanceRef;

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Manager Data")
    class UDataTable* ItemBaseDataTableRef;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Manager Data")
    UDataTable* EquipmentSubDataTableRef;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data Manager Data")
    UDataTable* ConsumableSubDataTableRef;


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