// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventorySlot.h"
#include "Item/ItemBase.h"
#include "Item/EquipSlotType.h"
#include "Item/EquippableItem.h"
#include "TmpInventoryData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTmpInventoryData
{
    GENERATED_BODY()

    // 인벤토리 슬롯 정보
    UPROPERTY()
    TArray<FInventorySlot> TempInventorySlots;

    // 인벤토리 슬롯 개수
    UPROPERTY()
    int32 TempInventorySlotQuantity = 0;

    // 장착된 아이템들 (직렬화를 위해 SoftObjectPath 등으로 변환해도 됨)
    UPROPERTY()
    TMap<EEquipSlotType, TSoftObjectPtr<UEquippableItem>> TempEquippedItems;
};
