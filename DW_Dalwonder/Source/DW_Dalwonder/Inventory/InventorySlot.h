#pragma once

#include "CoreMinimal.h"
#include "Item/itemData.h"
#include "Item/ItemBase.h"
#include "InventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemBase* ItemBase = nullptr; // 슬롯에 있는 아이템의 실제 UObject 인스턴스

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOccupied = false;

    FInventorySlot() { bIsOccupied = false; }

    bool IsEmpty() const
    {
        return ItemBase == nullptr || Quantity <= 0;
    }

    // 두 아이템 인스턴스가 같은 종류의 아이템인지 확인 (스택 여부 판단 시 사용)
    bool IsSameItemType(UItemBase* OtherItem) const
    {
        if (ItemBase == nullptr || OtherItem == nullptr) return false;
        return ItemBase->ItemCode == OtherItem->ItemCode; // ItemCode로 비교
    }
};
