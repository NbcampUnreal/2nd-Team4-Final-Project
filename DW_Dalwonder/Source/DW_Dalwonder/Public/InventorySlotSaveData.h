// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotSaveData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventorySlotSaveData
{
    GENERATED_BODY()

    // 아이템을 고유하게 식별하는 코드 (ex. 1001, 2002)
    UPROPERTY()
    int32 ItemCode = 0;

    // 해당 슬롯에 저장된 아이템 수량
    UPROPERTY()
    int32 Quantity = 0;
};
