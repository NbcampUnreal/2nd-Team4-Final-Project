// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NS_TraceSpawn.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UNS_TraceSpawn : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Connect Niagara")
	int32 ConnectIndex = 0;

	// 추가 데미지 설정입니다. 0이면 기본 공격력만큼 데미지를 입히며, -5면 기본 공격력에서 5를 뺀만큼 데미지를 입힙니다.
	UPROPERTY(EditAnywhere, Category = "Connect Niagara")
	float AddDamage = 0;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
