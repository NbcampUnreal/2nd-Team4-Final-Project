// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Telegraph/TelegraghActor.h"
#include "AnimNotify_SpawnTelegraph.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_SpawnTelegraph : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	// 스폰시킬 TelegraphActor 블루프린트 클래스입니다.
	UPROPERTY(EditAnywhere, Category = "Telegraph")
	TSubclassOf<AActor> TelegraphActorClass;

	// TelegraphActor 스폰 위치를 플레이어 대상으로 할지에 대한 변수입니다. True면 플레이어 중심으로 스폰시키고, False면 소유자 중심으로 스폰시킵니다.
	UPROPERTY(EditAnywhere, Category = "Telegraph")
	bool bIsTargeting = true;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
