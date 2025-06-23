// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "ConsumableItem.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UConsumableItem : public UItemBase
{
	GENERATED_BODY()

public:
	UConsumableItem();

	// UItemBase의 UseItem 함수를 오버라이드하여 소모품 고유의 동작을 구현합니다.
	virtual bool UseItem(AActor* Instigator) override;
	
};
