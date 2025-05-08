#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECharacterCombatState : uint8
{
	Idle            UMETA(DisplayName = "Idle"),
	Attacking       UMETA(DisplayName = "Attacking"),
	ComboWindow     UMETA(DisplayName = "Combo Window"),
	Parrying        UMETA(DisplayName = "Parrying"),
	Dodging         UMETA(DisplayName = "Dodging"),
	HitReact        UMETA(DisplayName = "Hit React"),
	Dead            UMETA(DisplayName = "Dead")
};