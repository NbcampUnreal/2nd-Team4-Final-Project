// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotifyState_SkeletonBossRotateToPlayer.h"

#include "Monster/BossMonster/Skeleton/DW_SkeletonBoss.h"

void UAnimNotifyState_SkeletonBossRotateToPlayer::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                                              UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ADW_SkeletonBoss* Boss = Cast<ADW_SkeletonBoss>(MeshComp->GetOwner()))
	{
		Boss->bRotateToPlayer = true;
	}
}

void UAnimNotifyState_SkeletonBossRotateToPlayer::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation)
{
	if (ADW_SkeletonBoss* Boss = Cast<ADW_SkeletonBoss>(MeshComp->GetOwner()))
	{
		Boss->bRotateToPlayer = false;
	}
}
