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

    UPROPERTY() float BaseMaxHealth;
    UPROPERTY() float BonusMaxHealth;

    UPROPERTY() float BaseAttack;
    UPROPERTY() float BonusAttack;

    UPROPERTY() float BaseDefense;
    UPROPERTY() float BonusDefense;

    UPROPERTY() float Weight;
    UPROPERTY() float BaseMaxWeight;
    UPROPERTY() float BonusMaxWeight;

    UPROPERTY() float BaseWalkSpeed;
    UPROPERTY() float BonusWalkSpeed;
    UPROPERTY() float SprintSpeed;
};
