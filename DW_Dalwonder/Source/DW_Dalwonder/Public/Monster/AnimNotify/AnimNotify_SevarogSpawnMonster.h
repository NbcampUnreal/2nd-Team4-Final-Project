// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Monster/DW_MonsterBase.h"
#include "AnimNotify_SevarogSpawnMonster.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_SevarogSpawnMonster : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	TSubclassOf<ADW_MonsterBase> SpawnMob;
	//이 Notify는 Sevarog에서만 실행되어야 함
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
