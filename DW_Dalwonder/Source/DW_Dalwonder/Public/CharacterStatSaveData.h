// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStatSaveData.generated.h"

/*

*/
USTRUCT(BlueprintType)
struct FCharacterStatSaveData
{
    GENERATED_BODY()

    // ==== 현재 상태 ====
    UPROPERTY() float Health = 100.f;
    UPROPERTY() float Stamina = 100.f;
    UPROPERTY() float CurrentWeight = 0.f;

    // ==== Base Stats ====
    UPROPERTY() float BaseMaxHealth = 100.f;
    UPROPERTY() float BaseHealthGenRate = 0.f;
    UPROPERTY() float BaseMaxStamina = 100.f;
    UPROPERTY() float BaseStaminaGenRate = 0.f;
    UPROPERTY() float BaseAttack = 0.f;
    UPROPERTY() float BaseDefense = 0.f;
    UPROPERTY() float BaseAttackSpeed = 0.f;
    UPROPERTY() float BaseWalkSpeed = 600.f;
    UPROPERTY() float BaseMaxWeight = 100.f;

    // ==== Equipment Bonus ====
    UPROPERTY() float EquipmentBonusMaxHealth = 0.f;
    UPROPERTY() float EquipmentBonusHealthGenRate = 0.f;
    UPROPERTY() float EquipmentBonusMaxStamina = 0.f;
    UPROPERTY() float EquipmentBonusStaminaGenRate = 0.f;
    UPROPERTY() float EquipmentBonusAttack = 0.f;
    UPROPERTY() float EquipmentBonusDefense = 0.f;
    UPROPERTY() float EquipmentBonusAttackSpeed = 0.f;
    UPROPERTY() float EquipmentBonusWalkSpeed = 0.f;
    UPROPERTY() float EquipmentBonusMaxWeight = 0.f;
};
