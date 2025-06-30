// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridArray.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGridArray
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FIntPoint> Grids;
};
