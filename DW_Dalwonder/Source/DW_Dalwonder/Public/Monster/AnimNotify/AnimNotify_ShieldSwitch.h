// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ShieldSwitch.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_ShieldSwitch : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Shield")
	bool bIsShieldOn = false;

	//�� Notify�� NormalMonster������ ����Ǿ�� ��
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
