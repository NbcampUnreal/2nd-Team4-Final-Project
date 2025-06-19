// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotifyState_SkeletonTrail.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Monster/BossMonster/Skeleton/DW_SkeletonBoss.h"


void UAnimNotifyState_SkeletonTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!TrailSystem || !MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	// 캐릭터가 ADW_SkeletonBoss인지 확인
	ADW_SkeletonBoss* Boss = Cast<ADW_SkeletonBoss>(Owner);
	if (!Boss || !Boss->RightHandWeaponComp) return;

	TrailComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		TrailSystem,
		Boss->RightHandWeaponComp,
		NAME_None,  // 소켓 없이 Weapon 루트에 부착
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true,
		true
	);
}

void UAnimNotifyState_SkeletonTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (TrailComp)
	{
		TrailComp->Deactivate();
		TrailComp->DestroyComponent();
		TrailComp = nullptr;
	}
}
