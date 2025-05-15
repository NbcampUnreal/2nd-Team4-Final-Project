// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Alert1500.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_Alert1500 : public UAnimNotify
{
	GENERATED_BODY()
				
public:

	//이 Notify는 NormalMonster에서만 실행되어야 함
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
