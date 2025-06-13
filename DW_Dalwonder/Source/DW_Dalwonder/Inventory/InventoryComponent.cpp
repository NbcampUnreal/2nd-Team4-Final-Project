#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::InitializeSlots()
{
    InventorySlots.Empty();

    for (int32 i = 0; i < InventorySlotQuantity; ++i)
    {
        InventorySlots.Add(FInventorySlot());
    }
}

bool UInventoryComponent::AddItem( UItemBase* ItemBase)
{
    int32 RemainingQuantity = 1;

    // 먼저 같은 아이템이 있고 스택이 덜 찬 슬롯을 찾아서 추가
    for (FInventorySlot& Slot : InventorySlots)
    {
        //Slot.bIsOccupied && Slot.ItemBase->ItemBaseData.ItemName.EqualTo(ItemBase->ItemBaseData.ItemName)
        if (Slot.bIsOccupied && Slot.ItemBase->ItemCode == ItemBase->ItemCode)
        {
            int32 StackRoom = Slot.ItemBase->ItemBaseData.MaxStackSize - Slot.Quantity;
            if (StackRoom > 0)
            {
                int32 ToAdd = FMath::Min(StackRoom, RemainingQuantity);
                Slot.Quantity += ToAdd;
                RemainingQuantity -= ToAdd;

                //ShowInventoryStatus();
                if (RemainingQuantity <= 0)
                    return true;
            }
        }
    }

    // 남은 수량이 있으면 빈 슬롯을 찾아 새로 넣기
    for (FInventorySlot& Slot : InventorySlots)
    {
        if (!Slot.bIsOccupied)
        {
            if (Slot.ItemBase == nullptr)
            {
				Slot.ItemBase = NewObject<UItemBase>(this); // 아이템 베이스 객체 생성
            }

            int32 ToAdd = FMath::Min(ItemBase->ItemBaseData.MaxStackSize, RemainingQuantity);

            Slot.ItemBase = ItemBase;
            Slot.Quantity = ToAdd;
            Slot.bIsOccupied = true;

            RemainingQuantity -= ToAdd;

            //ShowInventoryStatus();
            if (RemainingQuantity <= 0)
                return true;
        }
    }

    // 남은 수량이 처리되지 못함 (슬롯 부족)
    if (RemainingQuantity > 0)
    {
        /*if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
                FString::Printf(TEXT("인벤토리 가득참!!!"),
                    RemainingQuantity, *ItemData.ItemName.ToString()));
        }
        ShowInventoryStatus();*/
        return false;
    }

    return true;
}

void UInventoryComponent::ShowInventoryStatus()
{
    if (!GEngine) return;

    for (int32 i = 0; i < InventorySlots.Num(); ++i)
    {
        const FInventorySlot& Slot = InventorySlots[i];
        FString Msg;

        if (Slot.bIsOccupied)
        {
            Msg = FString::Printf(TEXT("[Slot %d] %s x%d"),
                i,
                *Slot.ItemBase->ItemBaseData.ItemName.ToString(),
                Slot.Quantity);
        }
        else
        {
            Msg = FString::Printf(TEXT("[Slot %d] (Empty)"), i);
        }

#if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, Msg);
#endif
    }
}
