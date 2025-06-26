#include "Item/ConsumableItem.h"
#include "GameFramework/Character.h" 
#include "Engine/Engine.h"           
#include "Character/CharacterStatComponent.h" 

UConsumableItem::UConsumableItem()
{
    // 생성자
}

bool UConsumableItem::UseItem(AActor* Instigator)
{
    // 1. 아이템 사용 주체(Instigator)가 유효한지 확인
    if (!Instigator)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UConsumableItem::UseItem - Instigator is null."));
#endif
        return false;
    }

    // 2. Instigator에서 UCharacterStatComponent를 가져옵니다.
    // ADW_CharacterBase* Character = Cast<ADW_CharacterBase>(Instigator); // 만약 ADW_CharacterBase에 CharacterStatComponent가 직접 붙어 있다면
    UCharacterStatComponent* CharacterStats = Instigator->FindComponentByClass<UCharacterStatComponent>();
    if (!CharacterStats)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("UConsumableItem::UseItem - Instigator %s does not have a UCharacterStatComponent!"), *Instigator->GetName());
#endif
        return false;
    }

    bool bEffectApplied = false; // 하나라도 효과가 적용되었는지 추적

    // 3. FItemData에 정의된 ConsumableEffects 배열을 순회하며 각 효과 처리
    for (const FConsumableEffectData& Effect : ItemBaseData.ConsumableEffects)
    {
        switch (Effect.EffectType)
        {
        case EConsumableEffectType::HealHealth:
            CharacterStats->SetHealth(CharacterStats->GetHealth() + Effect.EffectValue);

#if WITH_EDITOR
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("%s used %s: Healed %f HP. Current HP: %f"), *Instigator->GetName(), *ItemBaseData.ItemName.ToString(), Effect.EffectValue, CharacterStats->GetHealth()));
            UE_LOG(LogTemp, Log, TEXT("UConsumableItem::UseItem - %s used %s: Healed %f HP. Current HP: %f"), *Instigator->GetName(), *ItemBaseData.ItemName.ToString(), Effect.EffectValue, CharacterStats->GetHealth());
#endif
            bEffectApplied = true;
            break;

        case EConsumableEffectType::RestoreStamina:
            CharacterStats->SetStamina(CharacterStats->GetStamina() + Effect.EffectValue);
#if WITH_EDITOR
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("%s used %s: Healed %f Mana. Current Mana: %f"), *Instigator->GetName(), *ItemBaseData.ItemName.ToString(), Effect.EffectValue, CharacterStats->GetStamina()));
            UE_LOG(LogTemp, Log, TEXT("UConsumableItem::UseItem - %s used %s: Healed %f Mana. Current Mana: %f"), *Instigator->GetName(), *ItemBaseData.ItemName.ToString(), Effect.EffectValue, CharacterStats->GetStamina());
#endif
            bEffectApplied = true;
            break;
        case EConsumableEffectType::BuffAttack:
        case EConsumableEffectType::BuffDefense:
        case EConsumableEffectType::BuffMaxHealth:
        case EConsumableEffectType::BuffMaxStamina:
        case EConsumableEffectType::BuffHealthGenRate:
        case EConsumableEffectType::BuffStaminaGenRate:
        case EConsumableEffectType::BuffWalkSpeed:
        case EConsumableEffectType::BuffMaxWeight:
        case EConsumableEffectType::BuffAttackSpeed:
            CharacterStats->ApplyStatBuff(Effect.EffectType, Effect.EffectValue, Effect.Duration);
            bEffectApplied = true;
            break;
        default:
#if WITH_EDITOR
            UE_LOG(LogTemp, Warning, TEXT("UConsumableItem::UseItem - Unhandled consumable effect type: %s for item %s."), *UEnum::GetValueAsString(Effect.EffectType), *ItemBaseData.ItemName.ToString());
#endif
            break;
        }
    }

    // 부모 클래스의 UseItem 함수를 호출하여 기본 동작(로그 출력)을 실행합니다.
    Super::UseItem(Instigator);

    return bEffectApplied; // 하나라도 효과가 적용되었다면 true 반환
}