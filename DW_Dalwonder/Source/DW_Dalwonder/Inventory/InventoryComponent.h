#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventorySlot.h"
#include "Delegates/DelegateCombinations.h"
#include "UObject/NoExportTypes.h"
#include "Item/EquipSlotType.h"
#include "Item/EquippableItem.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType) // 블루프린트에서 사용 가능하도록 BlueprintType 지정
struct FEquippedItemsMapWrapper
{
    GENERATED_BODY()

public:
    // 장착된 아이템들을 저장하는 TMap
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipped Items")
    TMap<EEquipSlotType, UEquippableItem*> EquippedItemsMap; // 이 맵이 델리게이트를 통해 전달될 실제 데이터

    // 기본 생성자 (필요하다면)
    FEquippedItemsMapWrapper() {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedItemsUpdated, const FEquippedItemsMapWrapper&, EquippedItemsWrapper);

class UCharacterStatComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DW_DALWONDER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
    UInventoryComponent();
    
    // 인벤토리 슬롯 (아이템 인스턴스와 수량 포함)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FInventorySlot> InventorySlots;

    // 인벤토리 최대 슬롯 수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InventorySlotQuantity = 60;

    // 장착된 아이템들 (장비 슬롯 타입 -> 아이템 인스턴스)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Equipped")
    TMap<EEquipSlotType, UEquippableItem*> EquippedItems;

    UPROPERTY(BlueprintAssignable, Category = "Inventory | Events")
    FOnEquippedItemsUpdated OnEquippedItemsUpdated;

protected:
    // 캐릭터의 스탯 컴포넌트 참조
    UPROPERTY()
    TObjectPtr<UCharacterStatComponent> CharacterStatComponent;

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void InitializeSlots();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(UItemBase* ItemTemplate, int32& QuantityToAdd);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 FindItemSlotIndex(UItemBase* Item);

    // {{인벤토리 아이템 상호작용 함수들}} //
    // 인벤토리 슬롯의 아이템을 사용하는 함수
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool UseItemInSlot(int32 SlotIndex);

    // 인벤토리 슬롯의 아이템을 장착하는 함수
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool EquipItemInSlot(int32 SlotIndex);

    // 장착된 아이템을 해제하고 인벤토리로 돌려놓는 함수
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool UnequipItemFromSlot(EEquipSlotType EquipSlotType);

    // 인벤토리 슬롯의 아이템을 버리는 함수 (나중에 월드에 드롭하는 로직 추가)
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool DropItemInSlot(int32 SlotIndex, int32 QuantityToDrop = 1);

    // 특정 장비 슬롯에 장착된 아이템을 가져오는 함수
    UFUNCTION(BlueprintPure, Category = "Inventory | Equipped")
    UEquippableItem* GetEquippedItem(EEquipSlotType SlotType) const;

    // 인벤토리 상태를 디버그 메시지로 출력하는 함수
    UFUNCTION(BlueprintCallable, Category = "Inventory | Debug")
        void ShowInventoryStatus();
};
