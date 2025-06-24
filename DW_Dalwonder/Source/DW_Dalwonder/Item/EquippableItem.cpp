#include "Item/EquippableItem.h"
#include "GameFramework/Character.h" 
#include "Engine/Engine.h"           
#include "Components/SkeletalMeshComponent.h"
#include "Character/CharacterStatComponent.h" 

UEquippableItem::UEquippableItem()
{
    // 생성자
}

bool UEquippableItem::EquipItem(AActor* Instigator)
{
//    if (!Instigator)
//    {
//#if WITH_EDITOR
//        UE_LOG(LogTemp, Warning, TEXT("UEquippableItem::EquipItem - Instigator is null."));
//#endif
//        return false;
//    }
//
//    // Instigator에서 UCharacterStatComponent를 가져옵니다.
//    UCharacterStatComponent* CharacterStats = Instigator->FindComponentByClass<UCharacterStatComponent>();
//    if (!CharacterStats)
//    {
//#if WITH_EDITOR
//        UE_LOG(LogTemp, Warning, TEXT("UEquippableItem::EquipItem - Instigator %s does not have a UCharacterStatComponent!"), *Instigator->GetName());
//#endif
//        return false;
//    }
//
//    // 1. ItemBaseData의 스탯(AttackPower, DefensePower)을 캐릭터에 적용
//    CharacterStats->SetBonusAttack(CharacterStats->GetBonusAttack() + ItemBaseData.AttackPower);
//    CharacterStats->SetBonusDefense(CharacterStats->GetBonusDefense() + ItemBaseData.DefensePower);
//
//#if WITH_EDITOR
//    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("%s equipped %s: Attack %f, Defense %f."), *Instigator->GetName(), *ItemBaseData.ItemName.ToString(), ItemBaseData.AttackPower, ItemBaseData.DefensePower));
//#endif
//    UE_LOG(LogTemp, Log, TEXT("UEquippableItem::EquipItem - %s equipped %s (ATK: %f, DEF: %f)."), *Instigator->GetName(), *ItemBaseData.ItemName.ToString(), ItemBaseData.AttackPower, ItemBaseData.DefensePower);
//
//    // 2. ItemBaseData의 EquipSlot과 AttachSocketName을 기반으로 외형 변경 로직 처리
//    ACharacter* PlayerCharacter = Cast<ACharacter>(Instigator);
//    if (PlayerCharacter && PlayerCharacter->GetMesh()) // 캐릭터의 스켈레탈 메쉬가 있는지 확인
//    {
//        if (ItemBaseData.ItemType == EItemType::Equipment)
//        {
//            if (ItemBaseData.EquipSlot == EEquipSlotType::MainHand || ItemBaseData.EquipSlot == EEquipSlotType::TwoHanded || ItemBaseData.EquipSlot == EEquipSlotType::OffHand)
//            {
//                // TODO: 무기 메쉬를 로드하고 캐릭터의 스켈레탈 메쉬에 부착하는 로직
//                // ItemBaseData에 UStaticMesh* 또는 USkeletalMesh* WeaponMesh와 같은 멤버가 있어야 합니다.
//                // PlayerCharacter->GetMesh()->AttachComponentToSocket(WeaponMeshComponent, ItemBaseData.AttachSocketName);
//                // 지금은 로직만 표시합니다.
//#if WITH_EDITOR
//                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, FString::Printf(TEXT("Attaching weapon mesh for %s to %s's %s socket."), *ItemBaseData.ItemName.ToString(), *Instigator->GetName(), *ItemBaseData.AttachSocketName.ToString()));
//#endif
//                UE_LOG(LogTemp, Log, TEXT("UEquippableItem::EquipItem - Attaching weapon mesh for %s to %s."), *ItemBaseData.ItemName.ToString(), *ItemBaseData.AttachSocketName.ToString());
//            }
//            else if (ItemBaseData.EquipSlot == EEquipSlotType::Head || ItemBaseData.EquipSlot == EEquipSlotType::Torso || ItemBaseData.EquipSlot == EEquipSlotType::Legs || ItemBaseData.EquipSlot == EEquipSlotType::Feet || ItemBaseData.EquipSlot == EEquipSlotType::Hands || ItemBaseData.EquipSlot == EEquipSlotType::Shoulders)
//            {
//                // TODO: 방어구 메쉬를 로드하고 캐릭터의 스켈레탈 메쉬에 부착하는 로직
//                // ItemBaseData에 UStaticMesh* 또는 USkeletalMesh* ArmorMesh (슬롯별로 나눌 수도 있음)
//                // PlayerCharacter->GetMesh()->AttachComponentToSocket(ArmorMeshComponent, "YourArmorSocketName"); // 방어구는 보통 특정 소켓이 아닌 해당 부위를 가리는 방식
//#if WITH_EDITOR
//                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, FString::Printf(TEXT("Attaching armor mesh for %s to %s slot."), *ItemBaseData.ItemName.ToString(), *UEnum::GetValueAsString(ItemBaseData.EquipSlot)));
//#endif
//                UE_LOG(LogTemp, Log, TEXT("UEquippableItem::EquipItem - Attaching armor mesh for %s to %s slot."), *ItemBaseData.ItemName.ToString(), *UEnum::GetValueAsString(ItemBaseData.EquipSlot));
//            }
//        }
//    }
//
//
//    Super::EquipItem(Instigator);
//    return true; // 장착 성공
//}
//
//bool UEquippableItem::UnequipItem(AActor* Instigator)
//{
//    if (!Instigator)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("UEquippableItem::UnequipItem - Instigator is null."));
//        return false;
//    }
//
//    // Instigator에서 UCharacterStatComponent를 가져옵니다.
//    UCharacterStatComponent* CharacterStats = Instigator->FindComponentByClass<UCharacterStatComponent>();
//    if (!CharacterStats)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("UEquippableItem::UnequipItem - Instigator %s does not have a UCharacterStatComponent!"), *Instigator->GetName());
//        return false;
//    }
//
//    // 1. ItemBaseData의 스탯(AttackPower, DefensePower)을 캐릭터에서 제거
//    CharacterStats->SetBonusAttack(CharacterStats->GetBonusAttack() - ItemBaseData.AttackPower);
//    CharacterStats->SetBonusDefense(CharacterStats->GetBonusDefense() - ItemBaseData.DefensePower);
//    // TODO: BaseMaxHealth, BaseMaxStamina, BaseWalkSpeed 등 장비로 인한 다른 스탯 변화도 여기에 제거 로직 추가
//
//#if WITH_EDITOR
//    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, FString::Printf(TEXT("%s unequipped %s: ATK %f, DEF %f removed."), *Instigator->GetName(), *ItemBaseData.ItemName.ToString(), ItemBaseData.AttackPower, ItemBaseData.DefensePower));
//#endif
//    UE_LOG(LogTemp, Log, TEXT("UEquippableItem::UnequipItem - %s unequipped %s (ATK: %f, DEF: %f removed)."), *Instigator->GetName(), *ItemBaseData.ItemName.ToString(), ItemBaseData.AttackPower, ItemBaseData.DefensePower);
//
//    // 2. 외형 변경 로직 (부착된 메쉬 분리)
//    ACharacter* PlayerCharacter = Cast<ACharacter>(Instigator);
//    if (PlayerCharacter && PlayerCharacter->GetMesh())
//    {
//        if (ItemBaseData.ItemType == EItemType::Equipment)
//        {
//            if (ItemBaseData.EquipSlot == EEquipSlotType::MainHand || ItemBaseData.EquipSlot == EEquipSlotType::TwoHanded || ItemBaseData.EquipSlot == EEquipSlotType::OffHand)
//            {
//                // TODO: 부착된 무기 메쉬를 분리하거나 숨기는 로직
//                // PlayerCharacter->GetMesh()->DetachComponentFromSocket(WeaponMeshComponent, ItemBaseData.AttachSocketName);
//                // 또는 PlayerCharacter->HideWeaponMesh();
//#if WITH_EDITOR
//                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Detaching weapon mesh for %s from %s's %s socket."), *ItemBaseData.ItemName.ToString(), *Instigator->GetName(), *ItemBaseData.AttachSocketName.ToString()));
//#endif
//                UE_LOG(LogTemp, Log, TEXT("UEquippableItem::UnequipItem - Detaching weapon mesh for %s from %s."), *ItemBaseData.ItemName.ToString(), *ItemBaseData.AttachSocketName.ToString());
//            }
//            else if (ItemBaseData.EquipSlot == EEquipSlotType::Head || ItemBaseData.EquipSlot == EEquipSlotType::Torso || ItemBaseData.EquipSlot == EEquipSlotType::Legs || ItemBaseData.EquipSlot == EEquipSlotType::Feet || ItemBaseData.EquipSlot == EEquipSlotType::Hands || ItemBaseData.EquipSlot == EEquipSlotType::Shoulders)
//            {
//                // TODO: 부착된 방어구 메쉬를 분리하거나 숨기는 로직
//                // PlayerCharacter->GetMesh()->DetachComponentFromSocket(ArmorMeshComponent, "YourArmorSocketName");
//                // 또는 PlayerCharacter->HideArmorMesh(ItemBaseData.EquipSlot);
//#if WITH_EDITOR
//                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Gray, FString::Printf(TEXT("Detaching armor mesh for %s from %s slot."), *ItemBaseData.ItemName.ToString(), *UEnum::GetValueAsString(ItemBaseData.EquipSlot)));
//#endif
//                UE_LOG(LogTemp, Log, TEXT("UEquippableItem::UnequipItem - Detaching armor mesh for %s from %s slot."), *ItemBaseData.ItemName.ToString(), *UEnum::GetValueAsString(ItemBaseData.EquipSlot));
//            }
//            // 악세서리 슬롯 등 기타 슬롯 해제 로직 추가
//        }
//    }
//
//    Super::UnequipItem(Instigator);
    return true; // 해제 성공
}

bool UEquippableItem::UnequipItem(AActor* Instigator)
{
    return false;
}
