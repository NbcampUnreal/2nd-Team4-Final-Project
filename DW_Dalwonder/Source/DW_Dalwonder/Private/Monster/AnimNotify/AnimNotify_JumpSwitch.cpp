// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_JumpSwitch.h"
#include "Monster/NormalMonster/WoodMonster/Mob_WoodGolem.h"

void UAnimNotify_JumpSwitch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		AMob_WoodGolem* Monster = Cast<AMob_WoodGolem>(Owner);

		if (IsValid(Monster))
		{
			if (bIsJumpOn)
			{
				Monster->JumpOn();
			}
			else
			{
				Monster->JumpOff();
			}
		}
	}
}
