#pragma once
#include "CoreMinimal.h"
#include "EquipSlotType.generated.h"

UENUM(BlueprintType)
enum class EEquipSlotType : uint8
{
	None		UMETA(DisplayName = "None"),
	Weapon		UMETA(DisplayName = "Weapon"),
	Helmet		UMETA(DisplayName = "Helmet"),
	Gloves		UMETA(DisplayName = "Gloves"),
	Chest		UMETA(DisplayName = "Chest"),
	Legs		UMETA(DisplayName = "Legs"),
	Glove		UMETA(DisplayName = "Glove"),
	Boots		UMETA(DisplayName = "Boots"),
	MAX			UMETA(Hidden), // 최대값은 숨김 처리
};