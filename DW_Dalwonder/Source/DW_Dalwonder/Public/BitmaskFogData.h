// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BitmaskFogData.generated.h"

/**
 * 
 */
USTRUCT()
struct FBitmaskFogData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Width; // 가로 그리드 수

    UPROPERTY()
    int32 Height; // 세로 그리드 수

    UPROPERTY()
    TArray<uint8> Bitmask; // 밝힌 여부 저장 (1비트당 1칸)
};
