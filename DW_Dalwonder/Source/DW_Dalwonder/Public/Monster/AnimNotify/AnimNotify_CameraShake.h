// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraShake")
	TSubclassOf<UCameraShakeBase> ShakeClass;

	// 진동 강도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraShake")
	float Scale = 1.f;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
