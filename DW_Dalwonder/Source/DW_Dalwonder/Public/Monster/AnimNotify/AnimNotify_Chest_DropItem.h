// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Chest_DropItem.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_Chest_DropItem : public UAnimNotify
{
	GENERATED_BODY()
			
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
