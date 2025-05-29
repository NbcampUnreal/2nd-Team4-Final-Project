// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_QuickShot.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_QuickShot : public UAnimNotify
{
	GENERATED_BODY()

public:
	// 즉발기로 사용할 TelegraphActor 블루프린트 클래스입니다.
	//UPROPERTY(EditAnywhere, Category = "Quick Shot Niagara")
	//TSubclassOf<ATelegraghActor> TelegraphActorClass;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
