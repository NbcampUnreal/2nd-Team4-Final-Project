#include "Item/ItemDataManager.h"


// static 멤버 변수 초기화
UItemDataManager* UItemDataManager::ItemDataManagerInstance = nullptr;

UItemDataManager::UItemDataManager()
{

}

UItemDataManager* UItemDataManager::GetInstance()
{
    if (ItemDataManagerInstance == nullptr)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Error, TEXT("ItemDataManager::GetInstance() called before the instance was set by UGameInstance."));
#endif
	}
    return ItemDataManagerInstance;
}

void UItemDataManager::SetInstance(UItemDataManager* InInstance)
{
    if (ItemDataManagerInstance != nullptr && InInstance != nullptr && ItemDataManagerInstance != InInstance)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("Attempted to set ItemDataManagerInstance multiple times or with a different instance. Ignoring."));
#endif
		return;
    }

    if (InInstance == nullptr && ItemDataManagerInstance != nullptr)
    {
        ItemDataManagerInstance->RemoveFromRoot(); // GC 보호 해제
        ItemDataManagerInstance = nullptr;
#if WITH_EDITOR
        UE_LOG(LogTemp, Log, TEXT("ItemDataManager instance unset."));
#endif
    }
    else if (InInstance != nullptr && ItemDataManagerInstance == nullptr)
    {
        ItemDataManagerInstance = InInstance;
        ItemDataManagerInstance->AddToRoot(); // GC로부터 보호 (핵심!)
#if WITH_EDITOR
        UE_LOG(LogTemp, Log, TEXT("ItemDataManager instance set."));
#endif
    }
}

void UItemDataManager::InitializeDataManager(class UDataTable* InItemBaseDataTable,
    class UDataTable* InEquipmentSubDataTable,
    class UDataTable* InConsumableSubDataTable)
{
    ItemBaseDataTable = InItemBaseDataTable;
    EquipmentSubDataTable = InEquipmentSubDataTable;
    ConsumableSubDataTable = InConsumableSubDataTable;


    // 데이터테이블 유효성 검사
    if (!ItemBaseDataTable) { 
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("ItemDataManager: ItemBaseDataTable is not assigned!"));
#endif
	}
    if (!EquipmentSubDataTable) { 
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("ItemDataManager: EquipmentSubDataTable is not assigned!"));
#endif
	}
    if (!ConsumableSubDataTable) { 
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("ItemDataManager: ConsumableSubDataTable is not assigned!")); 
#endif 
	}

#if WITH_EDITOR
    UE_LOG(LogTemp, Log, TEXT("ItemDataManager Initialized successfully."));
#endif
}

FItemData UItemDataManager::GetItemBaseData(FName ItemID, bool& bOutSuccess)
{
    bOutSuccess = false;
    if (ItemBaseDataTable)
    {
        FString ContextString = TEXT("GetItemBaseData");
        FItemData* FoundData = ItemBaseDataTable->FindRow<FItemData>(ItemID, ContextString, false);
        if (FoundData)
        {
            bOutSuccess = true;
            return *FoundData;
        }
    }
#if WITH_EDITOR
    UE_LOG(LogTemp, Warning, TEXT("Failed to find ItemBaseData for ItemID: %s"), *ItemID.ToString());
#endif
	return FItemData();
}

FGenericItemSubData UItemDataManager::GetItemSubDataByType(FName ItemID, EItemType ItemType, bool& bOutSuccess)
{
    FGenericItemSubData ResultData;
    bOutSuccess = false;

    switch (ItemType)
    {
    case EItemType::Equipment:
    {
        bool bSubSuccess;
        ResultData.EquipmentData = GetSubData<FEquipmentSubData>(ItemID, bSubSuccess);
        if (bSubSuccess) { bOutSuccess = true; }
		else {
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("Failed to find EquipmentSubData for ItemID: %s"), *ItemID.ToString());
#endif
		}
		break;
    }
    case EItemType::Consumable:
    {
        bool bSubSuccess;
        ResultData.ConsumableData = GetSubData<FConsumableSubData>(ItemID, bSubSuccess);
        if (bSubSuccess) { bOutSuccess = true; }
        else { 
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("Failed to find ConsumableSubData for ItemID: %s"), *ItemID.ToString()); }
#endif
		break;
    }
    default:
#if WITH_EDITOR
        UE_LOG(LogTemp, Error, TEXT("Unsupported ItemType for SubData retrieval: %s for ItemID: %s"), *UEnum::GetValueAsString(ItemType), *ItemID.ToString());
#endif
		break;
    }

    return ResultData;
}


