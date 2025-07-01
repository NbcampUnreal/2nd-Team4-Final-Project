#include "InventoryComponent.h"
#include "Item/ConsumableItem.h" 
#include "Item/EquippableItem.h" 
#include "Character/CharacterStatComponent.h"
#include "Engine/Engine.h" 
#include <Item/ItemDataManager.h>
#include <Item/ItemTranslator.h>

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
    if (ItemTemplate->IsStackable()) // 스택 가능한 아이템만 해당
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

int32 UInventoryComponent::FindItemSlotIndex(UItemBase* Item)
{
    if (!IsValid(Item)) return -1;
    
    for (int32 i = 0; i < InventorySlots.Num(); i++)
    {
        if (!InventorySlots[i].IsEmpty() && InventorySlots[i].Quantity > 0)
        {
            if (InventorySlots[i].IsSameItemType(Item))
            {
                return i;
            }
        }
    }

    return -1;
}

bool UInventoryComponent::UseItemInSlot(int32 SlotIndex)
{
    if (!InventorySlots.IsValidIndex(SlotIndex) || InventorySlots[SlotIndex].IsEmpty())
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UseItemInSlot - Invalid slot index or slot is empty: %d"), SlotIndex);
#endif
        return false;
    }

    UItemBase* ItemBaseInstance = InventorySlots[SlotIndex].ItemBase; // 먼저 UItemBase*로 가져옵니다.
    if (!ItemBaseInstance) return false;

    UConsumableItem* ItemToUse = Cast<UConsumableItem>(ItemBaseInstance);
    if (!ItemToUse) // 캐스팅 실패 시, 즉 소비 아이템이 아닐 경우
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UseItemInSlot - Item %s in slot %d is not a consumable item. Cannot use."),
            *ItemBaseInstance->ItemBaseData.ItemName.ToString(), SlotIndex);
#endif
        return false;
    }

    // 아이템 사용 로직 호출 (Instigator는 인벤토리 컴포넌트의 Owner)
    bool bUsedSuccessfully = ItemToUse->UseItem(GetOwner());

    if (bUsedSuccessfully)
    {
        // 소모품이라면 수량 감소
        if (ItemToUse->ItemBaseData.ItemType == EItemType::Consumable)
        {
            InventorySlots[SlotIndex].Quantity--;
            if (InventorySlots[SlotIndex].Quantity <= 0)
            {
                // 아이템 수량이 0이 되면 슬롯 비우기 및 가비지 컬렉션 대상 지정
                InventorySlots[SlotIndex].ItemBase->MarkAsGarbage();
                InventorySlots[SlotIndex].ItemBase = nullptr;
                InventorySlots[SlotIndex].Quantity = 0; // 명시적으로 0으로 설정
#if WITH_EDITOR
                UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::UseItemInSlot - Item %s consumed and slot %d is now empty."), *ItemToUse->ItemBaseData.ItemName.ToString(), SlotIndex);
#endif
            }
            else
            {
#if WITH_EDITOR
                UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::UseItemInSlot - Item %s consumed. Remaining: %d"), *ItemToUse->ItemBaseData.ItemName.ToString(), InventorySlots[SlotIndex].Quantity);
#endif
            }
        }
        //UI 업데이트 이벤트 디스패치 (블루프린트 이벤트를 발생시키거나 델리게이트 사용)
        return true;
    }
    return false;
}

bool UInventoryComponent::EquipItemInSlot(int32 SlotIndex)
{
    if (!InventorySlots.IsValidIndex(SlotIndex) || InventorySlots[SlotIndex].IsEmpty())
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::EquipItemInSlot - Invalid slot index or slot is empty: %d"), SlotIndex);
#endif
        return false;
    }

    UItemBase* ItemToEquipBase = InventorySlots[SlotIndex].ItemBase;
    if (!ItemToEquipBase || ItemToEquipBase->ItemBaseData.ItemType != EItemType::Equipment)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::EquipItemInSlot - Item in slot %d (%s) is not an equipment item."), SlotIndex, ItemToEquipBase ? *ItemToEquipBase->ItemBaseData.ItemName.ToString() : TEXT("NULL"));
#endif
        return false;
    }

    UEquippableItem* ItemToEquip = Cast<UEquippableItem>(ItemToEquipBase);
    if (!ItemToEquip)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::EquipItemInSlot - Failed to cast item in slot %d to UEquippableItem for %s."), SlotIndex, *ItemToEquipBase->ItemBaseData.ItemName.ToString());
#endif
        return false;
    }

    EEquipSlotType TargetSlot = ItemToEquip->ItemBaseData.EquipSlot;
    if (TargetSlot == EEquipSlotType::None)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::EquipItemInSlot - Equipment item %s has no valid equip slot defined."), *ItemToEquip->ItemBaseData.ItemName.ToString());
#endif 
        return false;
    }

    // 1. 해당 장비 슬롯에 이미 아이템이 있는지 확인
    UItemBase* CurrentlyEquippedItem = GetEquippedItem(TargetSlot); 

    // 2. 기존 아이템 해제 (필요시)
    if (CurrentlyEquippedItem)
    {
        // 기존 아이템의 UnequipItem 호출 (외형만 복구)
        if (CurrentlyEquippedItem->UnequipItem(GetOwner()))
        {
            // 인벤토리로 다시 돌려놓기 
            int temp = 1;
            if (!AddItem(CurrentlyEquippedItem, temp)) // 1개만 해제한다고 가정
            {
#if WITH_EDITOR
                UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::EquipItemInSlot - Failed to re-add unequipped item %s to inventory. It might be lost!"), *CurrentlyEquippedItem->ItemBaseData.ItemName.ToString());
#endif 
                // TODO: 인벤토리가 가득 찼을 때 어떻게 처리할지 (바닥에 드롭 등)
                CurrentlyEquippedItem->MarkAsGarbage(); // 버려진 아이템으로 간주
                EquippedItems.Remove(TargetSlot); // 맵에서도 제거 (잃어버린 것으로 처리)
            }
            else
            {
                // 성공적으로 인벤토리에 추가되었으므로, EquippedItems 맵에서 제거
                EquippedItems.Remove(TargetSlot);
#if WITH_EDITOR
                UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::EquipItemInSlot - Successfully unequipped %s and moved back to inventory."), *CurrentlyEquippedItem->ItemBaseData.ItemName.ToString());
#endif 
            }
        }
        else
        {
#if WITH_EDITOR
            UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::EquipItemInSlot - Failed to unequip existing item %s. Aborting new item equip."), *CurrentlyEquippedItem->ItemBaseData.ItemName.ToString());
#endif 
            return false; // 기존 아이템 해제 실패 시, 새 아이템 장착도 중단
        }
    }

    // 3. 새 아이템 장착
    if (ItemToEquip->EquipItem(GetOwner())) // 새 아이템의 EquipItem 로직 호출 (외형 변경)
    {
        EquippedItems.Add(TargetSlot, ItemToEquip);

        InventorySlots[SlotIndex].ItemBase = nullptr;
        InventorySlots[SlotIndex].Quantity = 0;
        
#if WITH_EDITOR
        // ... (DEBUG 로그 출력 부분) ...
        UE_LOG(LogTemp, Log, TEXT("--- EquippedItems Map Status After Equip ---"));
        for (const auto& Pair : EquippedItems)
        {
            FString ItemName = TEXT("INVALID_ITEM_NULL");
            if (IsValid(Pair.Value))
            {
                ItemName = Pair.Value->ItemBaseData.ItemName.ToString();
            }

            FString EquipSlotName = UEnum::GetValueAsString(TEXT("/Script/DW_Dalwonder.EEquipSlotType"), Pair.Key);

            UE_LOG(LogTemp, Log, TEXT("  Slot: %s, Item: %s"),
                *EquipSlotName,
                *ItemName);
        }
        UE_LOG(LogTemp, Log, TEXT("------------------------------------------"));
#endif 

#if WITH_EDITOR
        UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::EquipItemInSlot - Equipped %s to %s slot. Inventory slot %d cleared."), *ItemToEquip->ItemBaseData.ItemName.ToString(), *UEnum::GetValueAsString(TargetSlot), SlotIndex);
#endif 
        FEquippedItemsMapWrapper Wrapper;
        Wrapper.EquippedItemsMap = EquippedItems; // 현재 맵을 래퍼에 복사
        OnEquippedItemsUpdated.Broadcast(Wrapper);
        return true;
    }
    else
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::EquipItemInSlot - Failed to equip new item %s."), *ItemToEquip->ItemBaseData.ItemName.ToString());
#endif 
        return false;
    }
}

bool UInventoryComponent::UnequipItemFromSlot(EEquipSlotType EquipSlotType)
{
    UEquippableItem* ItemToUnequip = GetEquippedItem(EquipSlotType);
    if (!ItemToUnequip)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::UnequipItemFromSlot - No item found in %s slot to unequip."), *UEnum::GetValueAsString(EquipSlotType));
#endif 
        return false;
    }

    // 1. 아이템의 UnequipItem 호출 (스탯 제거 및 외형 복구)
    if (ItemToUnequip->UnequipItem(GetOwner()))
    {
        // 2. 인벤토리에 다시 돌려놓기 (1개만 해제하므로 수량 1 전달)
        int temp = 1;
        if (AddItem(ItemToUnequip, temp)) // 1개만 해제하므로 수량 1 전달
        {
            EquippedItems.Remove(EquipSlotType); // 장착 맵에서 제거
#if WITH_EDITOR
            UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::UnequipItemFromSlot - Unequipped %s from %s slot and moved to inventory."), *ItemToUnequip->ItemBaseData.ItemName.ToString(), *UEnum::GetValueAsString(EquipSlotType));
#endif 
            FEquippedItemsMapWrapper Wrapper;
            Wrapper.EquippedItemsMap = EquippedItems; // 현재 맵을 래퍼에 복사
            OnEquippedItemsUpdated.Broadcast(Wrapper);
            //UI 업데이트 이벤트 디스패치
            return true;
        }
        else
        {
            // 인벤토리에 다시 넣는데 실패했다면 (인벤토리가 가득 찬 경우)
            // 에러를 출력하고 아이템을 다시 장착 맵에 유지하거나, 월드에 강제로 드롭하는 등의 처리 필요
#if WITH_EDITOR
            UE_LOG(LogTemp, Error, TEXT("UInventoryComponent::UnequipItemFromSlot - Inventory is full! Failed to re-add unequipped item %s. Item remains equipped."), *ItemToUnequip->ItemBaseData.ItemName.ToString());
#endif 
            EquippedItems.Add(EquipSlotType, ItemToUnequip); // 장착 맵에 다시 추가
			ItemToUnequip->EquipItem(GetOwner()); // 아이템을 다시 장착 상태로 유지
            FEquippedItemsMapWrapper Wrapper;
            Wrapper.EquippedItemsMap = EquippedItems; // 현재 맵을 래퍼에 복사
            OnEquippedItemsUpdated.Broadcast(Wrapper);
            return false; // 인벤토리 부족으로 인해 해제 실패
        }
    }
    return false;
}

bool UInventoryComponent::DropItemInSlot(int32 SlotIndex, int32 QuantityToDrop)
{
    if (!InventorySlots.IsValidIndex(SlotIndex) || InventorySlots[SlotIndex].IsEmpty())
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::DropItemInSlot - Invalid slot index or slot is empty: %d"), SlotIndex);
#endif
        return false;
    }

    if (QuantityToDrop <= 0 || QuantityToDrop > InventorySlots[SlotIndex].Quantity)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::DropItemInSlot - Invalid quantity to drop for slot %d: %d (Current: %d)"), SlotIndex, QuantityToDrop, InventorySlots[SlotIndex].Quantity);
#endif
        return false;
    }

    UItemBase* ItemToDrop = InventorySlots[SlotIndex].ItemBase;
    if (!ItemToDrop) return false;

    // 아이템 드롭 로직 호출 (현재는 로그만 출력)
    bool bDroppedSuccessfully = ItemToDrop->DropItem(GetOwner()); // ItemBase에서 DropItem 로직을 담당 (보통 그냥 true 반환)

    if (bDroppedSuccessfully)
    {
        InventorySlots[SlotIndex].Quantity -= QuantityToDrop;
        if (InventorySlots[SlotIndex].Quantity <= 0)
        {
            InventorySlots[SlotIndex].ItemBase->MarkAsGarbage();
            InventorySlots[SlotIndex].ItemBase = nullptr;
            InventorySlots[SlotIndex].Quantity = 0;
            UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::DropItemInSlot - Item %s (x%d) dropped and slot %d is now empty."), *ItemToDrop->ItemBaseData.ItemName.ToString(), QuantityToDrop, SlotIndex);
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("UInventoryComponent::DropItemInSlot - Item %s (x%d) dropped. Remaining: %d"), *ItemToDrop->ItemBaseData.ItemName.ToString(), QuantityToDrop, InventorySlots[SlotIndex].Quantity);
        }
        // UI 업데이트 이벤트 디스패치
        return true;
    }
    return false;
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

UEquippableItem* UInventoryComponent::GetEquippedItem(EEquipSlotType SlotType) const
{
    // FindRef는 키가 없으면 nullptr 반환
    return EquippedItems.FindRef(SlotType);
}


void UItemBase::LoadItemFromCode(FString InItemCode)
{
    ItemCode = InItemCode;

    EItemGrade ParsedGrade = EItemGrade::Normal;
    int32 ParsedEnchantLevel = 0;
    FString ParsedRowID;
    bool bSuccess = false;

#if WITH_EDITOR
    UE_LOG(LogTemp, Warning, TEXT("[LoadItemFromCode] Input ItemCode: %s"), *InItemCode);
#endif

    // 1. 코드 파싱
    UItemTranslator::ParseItemCode(ItemCode, ParsedGrade, ParsedEnchantLevel, ParsedRowID, bSuccess);

#if WITH_EDITOR
    if (bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("[LoadItemFromCode] Parsed Result - Grade: %d, Enchant: %d, RowID: %s"),
            (int32)ParsedGrade, ParsedEnchantLevel, *ParsedRowID);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[LoadItemFromCode] ParseItemCode FAILED for ItemCode: %s"), *InItemCode);
    }
#endif

    if (bSuccess)
    {
        ItemGrade = ParsedGrade;
        EnchantLevel = ParsedEnchantLevel;

        // 2. 아이템 데이터 로딩
        const FItemData* FoundData = UItemDataManager::GetInstance()->GetItemData(ParsedRowID);

        if (FoundData)
        {
            ItemBaseData = *FoundData;

#if WITH_EDITOR
            UE_LOG(LogTemp, Warning, TEXT("[LoadItemFromCode] Item Loaded Successfully: %s (RowID: %s)"),
                *ItemBaseData.ItemName.ToString(), *ParsedRowID);
#endif
        }
        else
        {
#if WITH_EDITOR
            UE_LOG(LogTemp, Error, TEXT("[LoadItemFromCode] ItemData NOT FOUND for RowID: %s"), *ParsedRowID);
#endif
            ItemBaseData = FItemData(); // 비어 있는 기본값으로 초기화
        }
    }
    else
    {
        ItemBaseData = FItemData(); // 파싱 실패 시 초기화
    }
}

