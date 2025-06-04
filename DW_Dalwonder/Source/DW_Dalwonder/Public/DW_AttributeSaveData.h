// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DW_AttributeSaveData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAttributeSaveData
{
    GENERATED_BODY()

    UPROPERTY() float BaseMaxStamina;
    UPROPERTY() float BaseMoveSpeed;
    UPROPERTY() float BaseMaxCarryWeight;
    UPROPERTY() float BaseMaxHealth;
    UPROPERTY() float BaseHealthRegen;
    UPROPERTY() float BaseStaminaRegen;
    UPROPERTY() float BaseLongswordXPMod;
    UPROPERTY() float BaseGreatswordXPMod;
    UPROPERTY() float BaseDefense;
};
