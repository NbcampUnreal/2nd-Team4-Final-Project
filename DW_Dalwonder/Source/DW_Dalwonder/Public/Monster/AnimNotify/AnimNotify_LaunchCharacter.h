// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_LaunchCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_LaunchCharacter : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Launch")
	float LaunchX = 50.f;
	UPROPERTY(EditAnywhere, Category = "Launch")
	float LaunchY = 0.f;
	UPROPERTY(EditAnywhere, Category = "Launch")
	float LaunchZ = 100.f;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
