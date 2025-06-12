// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Alert.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_Alert : public UAnimNotify
{
	GENERATED_BODY()
				
public:

	UPROPERTY(EditAnywhere, Category = "Alert")
	int32 AlertRange = 0;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
