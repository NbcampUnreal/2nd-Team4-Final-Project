#include "InventoryComponent.h"
#include "Item/ConsumableItem.h" 
#include "Item/EquippableItem.h" 
#include "Character/CharacterStatComponent.h"
#include "GameFramework/Character.h" 
#include "Engine/Engine.h" 

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* OwnerActor = GetOwner();
    if (OwnerActor)
    {
        CharacterStatComponent = OwnerActor->FindComponentByClass<UCharacterStatComponent>();
        if (!CharacterStatComponent)
        {
#if WITH_EDITOR
            UE_LOG(LogTemp, Error, TEXT("UInventoryComponent: No CharacterStatComponent found on %s! Item effects will not apply."), *OwnerActor->GetName());
#endif
        }
    }

    InitializeSlots();

    // 모든 장비 슬롯을 nullptr로 초기화합니다.
    // EEquipSlotType에 정의된 모든 열거형 값을 순회합니다.
    for (uint8 i = 0; i < static_cast<uint8>(EEquipSlotType::MAX); ++i)
    {
        EEquipSlotType SlotType = static_cast<EEquipSlotType>(i);
        if (SlotType != EEquipSlotType::None) // None 슬롯은 장비하지 않으므로 제외
        {
            EquippedItems.Add(SlotType, nullptr);
        }
    }
}

void UInventoryComponent::InitializeSlots()
{
    InventorySlots.Empty();
    InventorySlots.SetNum(InventorySlotQuantity);
}

bool UInventoryComponent::AddItem(UItemBase* ItemTemplate, int32& QuantityToAdd)
{
    if (!ItemTemplate || QuantityToAdd <= 0)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::AddItem - Invalid ItemTemplate or QuantityToAdd."));
#endif
        return false;
    }

    int32 RemainingQuantity = QuantityToAdd;

    // 1. 같은 아이템이 있고 스택이 덜 찬 슬롯을 찾아 추가
    if (ItemTemplate->ItemBaseData.MaxStackSize > 1) // 스택 가능한 아이템만 해당
    {
        for (FInventorySlot& Slot : InventorySlots)
        {
            if (!Slot.IsEmpty() && Slot.IsSameItemType(ItemTemplate))
            {
                int32 StackRoom = Slot.ItemBase->ItemBaseData.MaxStackSize - Slot.Quantity;
                if (StackRoom > 0)
                {
                    int32 ToAdd = FMath::Min(StackRoom, RemainingQuantity);
                    Slot.Quantity += ToAdd;
                    RemainingQuantity -= ToAdd;

                    if (RemainingQuantity <= 0)
                    {
#if WITH_EDITOR
                        UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::AddItem - Added %s (x%d) to existing stack. Remaining: %d"), *ItemTemplate->ItemBaseData.ItemName.ToString(), QuantityToAdd, RemainingQuantity);
#endif
                        return true;
                    }
                }
            }
        }
    }

    // 2. 남은 수량이 있으면 빈 슬롯을 찾아 새로 넣기
    for (FInventorySlot& Slot : InventorySlots)
    {
        if (Slot.IsEmpty())
        {
            UItemBase* NewItemInstance = nullptr;

            // 아이템 타입에 따라 적절한 자식 클래스 인스턴스 생성
            switch (ItemTemplate->ItemBaseData.ItemType)
            {
            case EItemType::Consumable:
                NewItemInstance = NewObject<UConsumableItem>(this);
                break;
            case EItemType::Equipment:
                NewItemInstance = NewObject<UEquippableItem>(this);
                break;
            case EItemType::Quest:
                NewItemInstance = NewObject<UItemBase>(this);
                break;
            case EItemType::Material:
                NewItemInstance = NewObject<UItemBase>(this);
                break;
            default:
                NewItemInstance = NewObject<UItemBase>(this);
                break;
            }

            if (NewItemInstance)
            {
                // ItemTemplate의 데이터를 새로 생성된 인스턴스에 복사 (깊은 복사)
                // ItemBaseData를 직접 복사하거나, ItemCode로 다시 로드하는 방식 사용
                NewItemInstance->LoadItemFromCode(ItemTemplate->ItemCode); // ItemCode로 데이터 로드
                NewItemInstance->ItemGrade = ItemTemplate->ItemGrade; // 등급 복사
                NewItemInstance->EnchantLevel = ItemTemplate->EnchantLevel; // 강화 레벨 복사

                int32 ToAdd = FMath::Min(NewItemInstance->ItemBaseData.MaxStackSize, RemainingQuantity);

                Slot.ItemBase = NewItemInstance;
                Slot.Quantity = ToAdd;
                RemainingQuantity -= ToAdd;
#if WITH_EDITOR
                UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::AddItem - Added new %s (x%d) to slot. Remaining: %d"), *NewItemInstance->ItemBaseData.ItemName.ToString(), ToAdd, RemainingQuantity);
#endif
                if (RemainingQuantity <= 0)
                {
                    return true;
                }
            }
            else
            {
#if WITH_EDITOR
                UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::AddItem - Failed to create new item instance for %s."), *ItemTemplate->ItemBaseData.ItemName.ToString());
#endif
                // NewObject 실패 시 RemainingQuantity를 그대로 두어 인벤토리 가득 참으로 처리
                return false;
            }
        }
    }

    // 남은 수량이 처리되지 못함 (슬롯 부족)
    if (RemainingQuantity > 0)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::AddItem - Inventory is full. Could not add %d of %s."), RemainingQuantity, *ItemTemplate->ItemBaseData.ItemName.ToString());
#endif
        QuantityToAdd = RemainingQuantity;
        return false;
    }

    return true; // 모든 아이템 추가 성공
}

bool UInventoryComponent::UseItemInSlot(int32 SlotIndex)
{
    return false;
}

bool UInventoryComponent::EquipItemInSlot(int32 SlotIndex)
{
    return false;
}

bool UInventoryComponent::UnequipItemFromSlot(EEquipSlotType EquipSlotType)
{
    return false;
}

bool UInventoryComponent::DropItemInSlot(int32 SlotIndex, int32 QuantityToDrop)
{
    return false;
}

void UInventoryComponent::UpdateEquippedStats()
{
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

UItemBase* UInventoryComponent::GetEquippedItem(EEquipSlotType SlotType) const
{
    if (EquippedItems.Contains(SlotType))
    {
        return EquippedItems[SlotType];
    }
    return nullptr;
}
