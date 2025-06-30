// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Arachnid_TerrableShock.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UArachnid_TerrableShock : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Effects")
	UMaterialInterface* DecalMaterial;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
