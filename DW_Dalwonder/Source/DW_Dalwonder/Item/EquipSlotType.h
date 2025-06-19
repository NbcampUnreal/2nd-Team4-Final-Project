#pragma once
#include "CoreMinimal.h"
#include "EquipSlotType.generated.h"

UENUM(BlueprintType)
enum class EEquipSlotType : uint8
{
	None		UMETA(DisplayName = "None"),
	Weapon		UMETA(DisplayName = "Weapon"),
	Helmet		UMETA(DisplayName = "Helmet"),
	Chest		UMETA(DisplayName = "Chest"),
	Legs		UMETA(DisplayName = "Legs"),
	Feet		UMETA(DisplayName = "Feet")
};