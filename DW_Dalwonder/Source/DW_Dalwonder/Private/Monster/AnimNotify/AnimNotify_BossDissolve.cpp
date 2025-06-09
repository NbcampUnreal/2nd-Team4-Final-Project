// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_BossDissolve.h"

#include "Monster/BossMonster/DW_BossMonsterBase.h"

void UAnimNotify_BossDissolve::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_BossMonsterBase* Monster = Cast<ADW_BossMonsterBase>(Owner);

		if (IsValid(Monster))
		{
			Monster->DoDissolve(DissolveTime);
		}
	}
}
