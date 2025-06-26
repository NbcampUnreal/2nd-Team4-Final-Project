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

    UPROPERTY() float TotalMaxHealth;
    UPROPERTY() float TotalHealthGenRate;
    UPROPERTY() float TotalMaxStamina;
    UPROPERTY() float TotalStaminaGenRate;
    UPROPERTY() float TotalAttack;
	UPROPERTY() float TotalDefense;
	UPROPERTY() float TotalAttackSpeed;
	UPROPERTY() float TotalWalkSpeed;
	UPROPERTY() float TotalMaxWeight;

    UPROPERTY() float BaseMaxHealth;
	UPROPERTY() float BaseHealthGenRate;
	UPROPERTY() float BaseMaxStamina;
	UPROPERTY() float BaseStaminaGenRate;
    UPROPERTY() float BaseAttack;
    UPROPERTY() float BaseDefense;
    UPROPERTY() float BaseAttackSpeed;
    UPROPERTY() float BaseWalkSpeed;
    UPROPERTY() float BaseMaxWeight;

    UPROPERTY() float BuffBonusMaxHealth;
	UPROPERTY() float BuffBonusHealthGenRate;
	UPROPERTY() float BuffBonusMaxStamina;
	UPROPERTY() float BuffBonusStaminaGenRate;
	UPROPERTY() float BuffBonusAttack;
	UPROPERTY() float BuffBonusDefense;
	UPROPERTY() float BuffBonusAttackSpeed;
	UPROPERTY() float BuffBonusWalkSpeed;
	UPROPERTY() float BuffBonusMaxWeight;

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
