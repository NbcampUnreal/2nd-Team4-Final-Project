#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventorySlot.h"
#include "Item/EquipSlotType.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DW_DALWONDER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
    UInventoryComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FInventorySlot> InventorySlots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InventorySlotQuantity = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Equipped")
    TMap<EEquipSlotType, UItemBase*> EquippedItems;

    UFUNCTION(BlueprintCallable)
    void InitializeSlots();

    UFUNCTION(BlueprintCallable)
    bool AddItem(const FItemData& ItemData);


    UFUNCTION(BlueprintPure, Category = "Inventory | Equipped")
    UItemBase* GetEquippedItem(EEquipSlotType SlotType) const;

    void ShowInventoryStatus();


		
};
