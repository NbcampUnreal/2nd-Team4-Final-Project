// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ArachnidChangeTrace.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UArachnidChangeTrace : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	// 0 : Front, 1 : Right, 2 : Left
	UPROPERTY(EditAnywhere, Category = "Value")
	int32 Value = 0;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
