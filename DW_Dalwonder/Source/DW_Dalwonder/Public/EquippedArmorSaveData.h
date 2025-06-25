// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquippedArmorSaveData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEquippedArmorSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 HelmetCode = 0;

    UPROPERTY()
    int32 ArmorCode = 0;

    UPROPERTY()
    int32 GloveCode = 0;

    UPROPERTY()
    int32 BootsCode = 0;

    UPROPERTY()
    int32 WeaponCode = 0;
};
