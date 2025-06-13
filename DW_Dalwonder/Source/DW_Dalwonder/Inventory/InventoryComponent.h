#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventorySlot.h"
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
    int32 InventorySlotQuantity = 60;

    UFUNCTION(BlueprintCallable)
    void InitializeSlots();

    UFUNCTION(BlueprintCallable)
    bool AddItem( UItemBase* ItemBase);

    void ShowInventoryStatus();
		
};
