// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/SoftObjectPtr.h"
#include "Item/ItemBase.h"
#include "TmpArmorData.generated.h"

/**
* 
*/
USTRUCT(BlueprintType)
struct FTmpArmorData
{
   GENERATED_BODY()

   UPROPERTY()
   TSoftObjectPtr<UItemBase> Helmet;

   UPROPERTY()
   TSoftObjectPtr<UItemBase> Armor;

   UPROPERTY()
   TSoftObjectPtr<UItemBase> Glove;

   UPROPERTY()
   TSoftObjectPtr<UItemBase> Boots;

   UPROPERTY()
   TSoftObjectPtr<UItemBase> Weapon;
};
