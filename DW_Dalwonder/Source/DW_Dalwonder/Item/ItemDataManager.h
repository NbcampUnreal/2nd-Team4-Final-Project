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


	// 아이템 서브 데이터 테이블에서 데이터를 가져오는 함수 (템플릿을 사용하여 다양한 서브 데이터 타입을 처리)
    template<typename TBaseSubData>
    TBaseSubData GetSubData(FName ItemID, bool& bOutSuccess)
    {
        TBaseSubData ResultData = TBaseSubData();
        UDataTable* TargetDataTable = nullptr;

        if (TBaseSubData::StaticStruct() == FEquipmentSubData::StaticStruct())
        {
            TargetDataTable = EquipmentSubDataTable;
        }
        else if (TBaseSubData::StaticStruct() == FConsumableSubData::StaticStruct())
        {
            TargetDataTable = ConsumableSubDataTable;
        }
        else
        {
#if WITH_EDITOR
            UE_LOG(LogTemp, Warning, TEXT("GetSubData: Unsupported TBaseSubData type requested."));
#endif
			bOutSuccess = false;
            return ResultData;
        }

        if (TargetDataTable)
        {
            FString ContextString = TEXT("GetSubData");
            TBaseSubData* FoundData = TargetDataTable->FindRow<TBaseSubData>(ItemID, ContextString, false);
            if (FoundData)
            {
                ResultData = *FoundData;
                bOutSuccess = true;
                return ResultData;
            }
            else
            {
#if WITH_EDITOR
                UE_LOG(LogTemp, Warning, TEXT("GetSubData: ItemID '%s' not found in data table for type."), *ItemID.ToString());
#endif
			}
        }
        else
        {
#if WITH_EDITOR
            UE_LOG(LogTemp, Error, TEXT("GetSubData: Target DataTable is not assigned for requested type."));
#endif
		}

        bOutSuccess = false;
        return ResultData;
    }

    // 아이템 타입에 따라 적절한 서브 데이터 테이블에서 정보를 가져오는 함수 (BP 호출용 Wrapper)
    UFUNCTION(BlueprintCallable, Category = "Item Data Manager", Meta = (DisplayName = "Get Item Sub Data by Type", ExpandEnumAsExecs = "ItemType"))
    FGenericItemSubData GetItemSubDataByType(FName ItemID, EItemType ItemType, bool& bOutSuccess);
};
