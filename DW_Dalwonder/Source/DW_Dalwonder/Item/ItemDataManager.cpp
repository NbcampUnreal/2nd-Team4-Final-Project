#include "ItemDataManager.h"


// static 멤버 변수 초기화
UItemDataManager* UItemDataManager::ItemDataManagerInstance = nullptr;

UItemDataManager::UItemDataManager()
{

}

UItemDataManager* UItemDataManager::GetInstance()
{
    if (ItemDataManagerInstance == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemDataManager::GetInstance() called before the instance was set by UGameInstance."));
    }
    return ItemDataManagerInstance;
}

void UItemDataManager::SetInstance(UItemDataManager* InInstance)
{
    if (ItemDataManagerInstance != nullptr && InInstance != nullptr && ItemDataManagerInstance != InInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attempted to set ItemDataManagerInstance multiple times or with a different instance. Ignoring."));
        return;
    }

    if (InInstance == nullptr && ItemDataManagerInstance != nullptr)
    {
        ItemDataManagerInstance->RemoveFromRoot(); // GC 보호 해제
        ItemDataManagerInstance = nullptr;
        UE_LOG(LogTemp, Log, TEXT("ItemDataManager instance unset."));
    }
    else if (InInstance != nullptr && ItemDataManagerInstance == nullptr)
    {
        ItemDataManagerInstance = InInstance;
        ItemDataManagerInstance->AddToRoot(); // GC로부터 보호 (핵심!)
        UE_LOG(LogTemp, Log, TEXT("ItemDataManager instance set."));
    }
}

void UItemDataManager::InitializeDataManager(class UDataTable* InItemBaseDataTable)
{
    ItemBaseDataTable = InItemBaseDataTable;

    // 데이터테이블 유효성 검사
    if (!ItemBaseDataTable) { UE_LOG(LogTemp, Error, TEXT("ItemDataManager: ItemBaseDataTable is not assigned!")); }

    UE_LOG(LogTemp, Log, TEXT("ItemDataManager Initialized successfully."));
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
    UE_LOG(LogTemp, Warning, TEXT("Failed to find ItemBaseData for ItemID: %s"), *ItemID.ToString());
    return FItemData();
}


