// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_ShieldSwitch.h"
#include "Monster/NormalMonster/Skeleton/MobSkeleton.h"

void UAnimNotify_ShieldSwitch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		AMobSkeleton* Monster = Cast<AMobSkeleton>(Owner);

		if (IsValid(Monster))
		{
			if (bIsShieldOn)
			{
				Monster->ShieldOn();
			}
			else
			{
				Monster->ShieldOff();
			}
		}
	}
}
