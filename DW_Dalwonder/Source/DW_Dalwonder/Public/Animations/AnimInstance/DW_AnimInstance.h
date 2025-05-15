// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DW_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDW_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	bool bIsGuarding = false;
};
