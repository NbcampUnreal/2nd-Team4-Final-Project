// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_VisibleSwitch.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_VisibleSwitch : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Switch")
	bool bSetVisibility = true;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
