#include "ItemDataManager.h"
#include "Item/ItemTranslator.h"


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

void UItemDataManager::InitializeDataManager(class UDataTable* InItemBaseDataTable)
{
    ItemBaseDataTable = InItemBaseDataTable;

    // 데이터테이블 유효성 검사
    if (!ItemBaseDataTable) { 
#if WITH_EDITOR
        UE_LOG(LogTemp, Error, TEXT("ItemDataManager: ItemBaseDataTable is not assigned!")); 
#endif
    }

    if (!ItemBaseDataTable) { 
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("ItemDataManager: ItemBaseDataTable is not assigned!"));
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
    //UE_LOG(LogTemp, Warning, TEXT("Failed to find ItemBaseData for ItemID: %s"), *ItemID.ToString());
#endif
	return FItemData();
}

const FItemData* UItemDataManager::GetItemData(FString InRowID) const
{
    if (ItemBaseDataTable)
    {
        FName RowName = FName(*InRowID);
        return ItemBaseDataTable->FindRow<FItemData>(RowName, TEXT("GetItemData"));
    }
    //UE_LOG(LogTemp, Warning, TEXT("UItemDataManager: ItemDataTable is null. Cannot get item data for RowID %d."), InRowID);
    return nullptr;
}

FItemData UItemDataManager::GetItemDataFromCode(FString ItemCode)
{
    EItemGrade ParsedGrade;
    int32 ParsedEnchantLevel;
    FString ParsedRowID;
    bool bSuccess;

    UItemTranslator::ParseItemCode(ItemCode, ParsedGrade, ParsedEnchantLevel, ParsedRowID, bSuccess);

    if (bSuccess)
    {
        const FItemData* FoundData = GetItemData(ParsedRowID);
        if (FoundData)
        {
            return *FoundData;
        }
        else
        {
#if WITH_EDITOR
            //UE_LOG(LogTemp, Warning, TEXT("GetItemDataFromCode: Could not find item data for RowID %d (ItemCode: %d)."), ParsedRowID, ItemCode);
#endif
            return FItemData(); // 데이터 없으면 빈 FItemData 반환
        }
    }
    else
    {
#if WITH_EDITOR
        //UE_LOG(LogTemp, Error, TEXT("GetItemDataFromCode: Failed to parse ItemCode %d."), ItemCode);
#endif
        return FItemData(); // 파싱 실패 시 빈 FItemData 반환
    }
}

EItemGrade UItemDataManager::GetItemGradeFromCode(FString ItemCode)
{
    EItemGrade ParsedGrade = EItemGrade::Normal; // 기본값
    int32 ParsedEnchantLevel;
    FString ParsedRowID;
    bool bSuccess;

    UItemTranslator::ParseItemCode(ItemCode, ParsedGrade, ParsedEnchantLevel, ParsedRowID, bSuccess);

    if (!bSuccess)
    {
#if WITH_EDITOR
        //UE_LOG(LogTemp, Error, TEXT("GetItemGradeFromCode: Failed to parse ItemCode %d. Returning EItemGrade::Normal."), ItemCode);
#endif
        return EItemGrade::Normal; // 파싱 실패 시 기본 등급 반환
    }
    return ParsedGrade;
}

int32 UItemDataManager::GetEnchantLevelFromCode(FString ItemCode)
{
    EItemGrade ParsedGrade;
    int32 ParsedEnchantLevel = 0; // 기본값
    FString ParsedRowID;
    bool bSuccess;

    UItemTranslator::ParseItemCode(ItemCode, ParsedGrade, ParsedEnchantLevel, ParsedRowID, bSuccess);

    if (!bSuccess)
    {
#if WITH_EDITOR
        //UE_LOG(LogTemp, Error, TEXT("GetEnchantLevelFromCode: Failed to parse ItemCode %d. Returning 0."), ItemCode);
#endif
        return 0; // 파싱 실패 시 기본 강화 레벨 반환
    }
    return ParsedEnchantLevel;
}