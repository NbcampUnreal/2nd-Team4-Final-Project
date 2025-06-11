// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BossDissolve.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_BossDissolve : public UAnimNotify
{
	GENERATED_BODY()
			
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	float DissolveTime;
	//이 Notify는 BossMonster에서만 실행되어야 함
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
