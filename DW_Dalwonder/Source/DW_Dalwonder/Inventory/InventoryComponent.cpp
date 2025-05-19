#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::InitializeSlots()
{
    InventorySlots.Empty();

    for (int32 i = 0; i < 3; ++i)
    {
        InventorySlots.Add(FInventorySlot());
    }
}

bool UInventoryComponent::AddItem(const FItemData& ItemData)
{
    int32 RemainingQuantity = 1;

    // 먼저 같은 아이템이 있고 스택이 덜 찬 슬롯을 찾아서 추가
    for (FInventorySlot& Slot : InventorySlots)
    {
        if (Slot.bIsOccupied && Slot.Item.ItemData.ItemName.EqualTo(ItemData.ItemName))
        {
            int32 StackRoom = Slot.Item.ItemData.MaxStackSize - Slot.Item.Quantity;
            if (StackRoom > 0)
            {
                int32 ToAdd = FMath::Min(StackRoom, RemainingQuantity);
                Slot.Item.Quantity += ToAdd;
                RemainingQuantity -= ToAdd;

                ShowInventoryStatus();
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
            int32 ToAdd = FMath::Min(ItemData.MaxStackSize, RemainingQuantity);

            Slot.Item.ItemData = ItemData;
            Slot.Item.Quantity = ToAdd;
            Slot.bIsOccupied = true;

            RemainingQuantity -= ToAdd;

            ShowInventoryStatus();
            if (RemainingQuantity <= 0)
                return true;
        }
    }

    // 남은 수량이 처리되지 못함 (슬롯 부족)
    if (RemainingQuantity > 0)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
                FString::Printf(TEXT("인벤토리 가득참!!!"),
                    RemainingQuantity, *ItemData.ItemName.ToString()));
        }
        ShowInventoryStatus();
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
                *Slot.Item.ItemData.ItemName.ToString(),
                Slot.Item.Quantity);
        }
        else
        {
            Msg = FString::Printf(TEXT("[Slot %d] (Empty)"), i);
        }

        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, Msg);
    }
}
