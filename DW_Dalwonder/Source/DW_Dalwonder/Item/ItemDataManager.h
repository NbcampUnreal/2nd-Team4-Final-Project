#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ItemData.h"
#include "ItemDataManager.generated.h"

UCLASS(Blueprintable, BlueprintType)
class DW_DALWONDER_API UItemDataManager : public UObject
{
	GENERATED_BODY()
	
private:
    // 싱글턴 인스턴스
    static UItemDataManager* ItemDataManagerInstance;

public:
    UItemDataManager();

    UFUNCTION(BlueprintCallable, Category = "Item Data Manager", meta = (DisplayName = "Get Item Data Manager Instance"))
    static UItemDataManager* GetInstance();

    static void SetInstance(UItemDataManager* InInstance);

    UFUNCTION(BlueprintCallable, Category = "Item Data Manager")
    void InitializeDataManager(class UDataTable* InItemBaseDataTable);


	// 아이템 기본 데이터 테이블과 서브 데이터 테이블
    UPROPERTY()
    class UDataTable* ItemBaseDataTable;


    UFUNCTION(BlueprintCallable, Category = "Item Data Manager")
    FItemData GetItemBaseData(FName ItemID, bool& bOutSuccess);

};
