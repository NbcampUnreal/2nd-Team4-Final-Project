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
	// ��߱�� ����� TelegraphActor �������Ʈ Ŭ�����Դϴ�.
	//UPROPERTY(EditAnywhere, Category = "Quick Shot Niagara")
	//TSubclassOf<ATelegraghActor> TelegraphActorClass;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
