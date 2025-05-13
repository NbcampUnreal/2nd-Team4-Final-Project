// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DW_NotifyState_ParryWindow.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDW_NotifyState_ParryWindow : public UAnimNotifyState
{
	GENERATED_BODY()

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
