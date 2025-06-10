// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueLine.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDialogueLine
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText SpeakerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DialogueText;
};
