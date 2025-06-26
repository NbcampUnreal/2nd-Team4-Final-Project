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

	// 아이템을 장착할 때 호출되는 가상 함수를 오버라이드
	virtual bool EquipItem(AActor* Instigator) override;

	// 아이템을 해제할 때 호출되는 가상 함수를 오버라이드
	virtual bool UnequipItem(AActor* Instigator) override;

protected:
	// 이 아이템이 장착될 슬롯 타입입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equippable Item")
	EEquipSlotType EquipSlotType = EEquipSlotType::None;

public:
	// 장착 슬롯 타입 Getter
	UFUNCTION(BlueprintPure, Category = "Equippable Item")
	EEquipSlotType GetEquipSlotType() const { return EquipSlotType; }
};
