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

    UFUNCTION(BlueprintCallable)
    void InitializeSlots();

    UFUNCTION(BlueprintCallable)
    bool AddItem(const FItemData& ItemData);

    void ShowInventoryStatus();
		
};
