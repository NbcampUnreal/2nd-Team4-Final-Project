#pragma once
#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "EquippableItem.generated.h"

UCLASS()
class DW_DALWONDER_API UEquippableItem : public UItemBase
{
	GENERATED_BODY()
public:
	UEquippableItem();

	virtual bool EquipItem(AActor* Instigator) override;
	virtual bool UnequipItem(AActor* Instigator) override;
};
