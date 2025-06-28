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
    FString HelmetCode = "0";

    UPROPERTY()
    FString ArmorCode = "0";

    UPROPERTY()
    FString GloveCode = "0";

    UPROPERTY()
    FString BootsCode = "0";

    UPROPERTY()
    FString WeaponCode = "0";
};
