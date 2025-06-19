#include "Item/ItemBase.h"
#include "Item/ItemTranslator.h"
#include "ItemDataManager.h"

UItemBase::UItemBase()
{
    
}

void UItemBase::LoadItemFromCode(int32 InItemCode)
{
    ItemCode = InItemCode;

    EItemGrade ParsedGrade;
    int32 ParsedEnchantLevel;
    int32 ParsedRowID;
    bool bSuccess;

    // UItemTranslator를 사용하여 ItemCode 파싱
    UItemTranslator::ParseItemCode(ItemCode, ParsedGrade, ParsedEnchantLevel, ParsedRowID, bSuccess);

    if (bSuccess)
    {
        ItemGrade = ParsedGrade;
        EnchantLevel = ParsedEnchantLevel;

        // ItemDataManager를 통해 데이터를 가져옵니다.
        const FItemData* FoundData = UItemDataManager::GetInstance()->GetItemData(ParsedRowID);

        if (FoundData)
        {
            ItemBaseData = *FoundData;
#if WITH_EDITOR
            UE_LOG(LogTemp, Log, TEXT("Item %s (Code: %d) loaded successfully from Data Manager. Enchant: %d"), *ItemBaseData.ItemName.ToString(), ItemCode, EnchantLevel);
#endif
        }
        else
        {
#if WITH_EDITOR
            UE_LOG(LogTemp, Warning, TEXT("UItem::LoadItemFromCode: Could not find item data for RowID %d in Data Manager for code %d."), ParsedRowID, ItemCode);
#endif
            ItemBaseData = FItemData(); // 기본값 또는 에러 아이템 설정
        }
    }
    else
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Error, TEXT("UItem::LoadItemFromCode: Failed to parse ItemCode %d."), InItemCode);
#endif
        ItemBaseData = FItemData();// 파싱 실패 시 기본값 또는 에러 처리
    }
}

// --- 아이템 동작 기본 구현 ---
bool UItemBase::UseItem(AActor* Instigator)
{
#if WITH_EDITOR
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("%s (Code: %d) Used (Base Impl) by %s"), *ItemBaseData.ItemName.ToString(), ItemCode, Instigator ? *Instigator->GetName() : TEXT("None")));
    UE_LOG(LogTemp, Log, TEXT("UItemBase::UseItem - %s (Code: %d) Used by %s. (Base Implementation - No specific effect)"), *ItemBaseData.ItemName.ToString(), ItemCode, Instigator ? *Instigator->GetName() : TEXT("None"));
#endif
    return false; // 기본적으로 아무 효과 없음
}

bool UItemBase::EquipItem(AActor* Instigator)
{
#if WITH_EDITOR
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("%s (Code: %d) Equipped (Base Impl) by %s"), *ItemBaseData.ItemName.ToString(), ItemCode, Instigator ? *Instigator->GetName() : TEXT("None")));
    UE_LOG(LogTemp, Log, TEXT("UItemBase::EquipItem - %s (Code: %d) Equipped by %s. (Base Implementation - No specific effect)"), *ItemBaseData.ItemName.ToString(), ItemCode, Instigator ? *Instigator->GetName() : TEXT("None"));
#endif
    return false; // 기본적으로 장착 불가능
}

bool UItemBase::UnequipItem(AActor* Instigator)
{
#if WITH_EDITOR
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("%s (Code: %d) Unequipped (Base Impl) by %s"), *ItemBaseData.ItemName.ToString(), ItemCode, Instigator ? *Instigator->GetName() : TEXT("None")));
    UE_LOG(LogTemp, Log, TEXT("UItemBase::UnequipItem - %s (Code: %d) Unequipped by %s. (Base Implementation - No specific effect)"), *ItemBaseData.ItemName.ToString(), ItemCode, Instigator ? *Instigator->GetName() : TEXT("None"));
#endif
    return false; // 기본적으로 해제 불가능
}

bool UItemBase::DropItem(AActor* Instigator)
{
#if WITH_EDITOR
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("%s (Code: %d) Dropped (Base Impl) by %s"), *ItemBaseData.ItemName.ToString(), ItemCode, Instigator ? *Instigator->GetName() : TEXT("None")));
    UE_LOG(LogTemp, Log, TEXT("UItemBase::DropItem - %s (Code: %d) Dropped by %s. (Base Implementation - No actual drop)"), *ItemBaseData.ItemName.ToString(), ItemCode, Instigator ? *Instigator->GetName() : TEXT("None"));
#endif
    // 실제 아이템을 월드에 스폰하는 로직은 나중에 추가
    return true; // 기본적으로 버리는 행위 자체는 성공으로 간주
}
