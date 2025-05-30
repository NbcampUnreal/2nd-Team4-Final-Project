// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CollisionSwitch.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_CollisionSwitch : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Collision Enable")
	bool Preset_NoCollision = true;
	UPROPERTY(EditAnywhere, Category = "Collision Enable")
	bool Preset_Monster = false;
	UPROPERTY(EditAnywhere, Category = "Collision Enable")
	bool Preset_Pawn = false;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
