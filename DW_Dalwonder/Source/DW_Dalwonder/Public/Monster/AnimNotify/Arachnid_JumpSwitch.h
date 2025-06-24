// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Arachnid_JumpSwitch.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UArachnid_JumpSwitch : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Switch")
	bool bIsJumpOn = false;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
