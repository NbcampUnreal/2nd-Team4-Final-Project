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

bool UInventoryComponent::AddItem(const FItemData& ItemData)
{
    int32 RemainingQuantity = 1;

    // 먼저 같은 아이템이 있고 스택이 덜 찬 슬롯을 찾아서 추가
    for (FInventorySlot& Slot : InventorySlots)
    {
        if (Slot.bIsOccupied && Slot.ItemBase->ItemBaseData.ItemName.EqualTo(ItemData.ItemName))
        {
            int32 StackRoom = Slot.ItemBase->ItemBaseData.MaxStackSize - Slot.Quantity;
            if (StackRoom > 0)
            {
                int32 ToAdd = FMath::Min(StackRoom, RemainingQuantity);
                Slot.Quantity += ToAdd;
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

            // !!! 핵심 수정 부분 !!!
            // 빈 슬롯을 찾았을 때 UItemBase 객체를 생성하여 할당합니다.
            if (Slot.ItemBase == nullptr)
            {
                Slot.ItemBase = NewObject<UItemBase>(this); // 이 InventoryComponent를 Outer로 사용
                // NewObject 호출 후 LoadItemFromCode를 통해 데이터를 로드하는 것이 좋습니다.
                // 다만 AddItem 함수가 이미 FItemData를 가지고 있으므로,
                // ItemBaseData를 직접 복사하는 방식으로 진행할 수 있습니다.
                // 만약 ItemBase가 ItemCode를 기반으로 데이터를 로드해야 한다면
                // Slot.ItemBase->LoadItemFromCode(ItemData.ItemCode); 와 같이 호출하세요.
            }

            int32 ToAdd = FMath::Min(ItemData.MaxStackSize, RemainingQuantity);

            Slot.ItemBase->ItemBaseData = ItemData;
            Slot.Quantity = ToAdd;
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
                *Slot.ItemBase->ItemBaseData.ItemName.ToString(),
                Slot.Quantity);
        }
        else
        {
            Msg = FString::Printf(TEXT("[Slot %d] (Empty)"), i);
        }

        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, Msg);
    }
}
