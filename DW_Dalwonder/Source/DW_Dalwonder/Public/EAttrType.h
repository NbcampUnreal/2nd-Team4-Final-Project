// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EAttrType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAttrType : uint8
{
    MaxHealth      UMETA(DisplayName = "Max Health"),
    MaxStamina     UMETA(DisplayName = "Max Stamina"),
    Defense        UMETA(DisplayName = "Defense"),
    HealthRegen    UMETA(DisplayName = "Health Regen"),
    StaminaRegen   UMETA(DisplayName = "Stamina Regen"),
    MoveSpeed      UMETA(DisplayName = "Move Speed"),
    MaxCarryWeight UMETA(DisplayName = "Max Carry Weight")
};
