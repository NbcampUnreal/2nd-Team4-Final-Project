// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DW_UAnimNotify_FootstepEffect_H.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDW_UAnimNotify_FootstepEffect_H : public UAnimNotify
{
	GENERATED_BODY()

public:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	FName FootSocketName = "";

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
