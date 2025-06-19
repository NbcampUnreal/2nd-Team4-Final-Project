#pragma once

#include "CoreMinimal.h"
#include "EConsumableEffectType.generated.h"

// 소비 아이템이 가질 수 있는 효과의 종류를 정의.
UENUM(BlueprintType)
enum class EConsumableEffectType : uint8
{
    None            UMETA(DisplayName = "None"),
    HealHealth      UMETA(DisplayName = "Heal Health"),       // 체력 회복
    RestoreStamina  UMETA(DisplayName = "Restore Stamina"),   // 스태미나 회복
    BuffAttack      UMETA(DisplayName = "Buff Attack"),       // 공격력 버프
    BuffDefense     UMETA(DisplayName = "Buff Defense")       // 방어력 버프
    // 필요한 소비 아이템 효과를 여기에 추가하세요.
};