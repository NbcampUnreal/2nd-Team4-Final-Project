#pragma once

#include "CoreMinimal.h"
#include "EConsumableEffectType.generated.h"

// 소비 아이템이 가질 수 있는 효과의 종류를 정의.
UENUM(BlueprintType)
enum class EConsumableEffectType : uint8
{
    None				UMETA(DisplayName = "None"),
    HealHealth			UMETA(DisplayName = "Heal Health"),				 // 체력 회복
    RestoreStamina		UMETA(DisplayName = "Restore Stamina"),			 // 스태미나 회복
    BuffAttack			UMETA(DisplayName = "Buff Attack"),				 // 공격력 버프
    BuffDefense			UMETA(DisplayName = "Buff Defense"),			 // 방어력 버프
	BuffMaxHealth		UMETA(DisplayName = "Buff Max Health"),			// 최대 체력 버프
	BuffMaxStamina		UMETA(DisplayName = "Buff Max Stamina"),		// 최대 스태미나 버프
	BuffHealthGenRate	UMETA(DisplayName = "Buff Health Gen Rate"),	// 체력 회복률 버프
	BuffStaminaGenRate	UMETA(DisplayName = "Buff Stamina Gen Rate"),	 // 스태미나 회복률 버프
	BuffWalkSpeed		UMETA(DisplayName = "Buff Walk Speed"),			 // 걷기 속도 버프
	BuffMaxWeight		UMETA(DisplayName = "Buff Max Weight"),			// 최대 소지 무게 버프
	BuffAttackSpeed		UMETA(DisplayName = "Buff Attack Speed")		// 공격 속도 버프
};