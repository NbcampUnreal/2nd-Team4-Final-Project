// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SevarogSetInvincible.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_SevarogSetInvincible : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	bool Invincible;
	//이 Notify는 Sevarog에서만 실행되어야 함
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
