// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TmpCharacterStatData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTmpCharacterStatData
{
    GENERATED_BODY()

    UPROPERTY() float Health;
    UPROPERTY() float Stamina;
    UPROPERTY() float CurrentWeight;

    UPROPERTY() float BaseMaxHealth;
	UPROPERTY() float BaseHealthGenRate;
	UPROPERTY() float BaseMaxStamina;
	UPROPERTY() float BaseStaminaGenRate;
    UPROPERTY() float BaseAttack;
    UPROPERTY() float BaseDefense;
    UPROPERTY() float BaseAttackSpeed;
    UPROPERTY() float BaseWalkSpeed;
    UPROPERTY() float BaseMaxWeight;

	UPROPERTY() float EquipmentBonusMaxHealth;
	UPROPERTY() float EquipmentBonusHealthGenRate;
	UPROPERTY() float EquipmentBonusMaxStamina;
	UPROPERTY() float EquipmentBonusStaminaGenRate;
	UPROPERTY() float EquipmentBonusAttack;
	UPROPERTY() float EquipmentBonusDefense;
	UPROPERTY() float EquipmentBonusAttackSpeed;
	UPROPERTY() float EquipmentBonusWalkSpeed;
	UPROPERTY() float EquipmentBonusMaxWeight;
};
