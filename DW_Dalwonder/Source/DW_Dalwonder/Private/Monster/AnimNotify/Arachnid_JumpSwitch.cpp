// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/Arachnid_JumpSwitch.h"
#include "Monster/BossMonster/Arachnid/BossArachnid.h"

void UArachnid_JumpSwitch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ABossArachnid* Monster = Cast<ABossArachnid>(Owner);

		if (IsValid(Monster))
		{
			if (bIsJumpOn)
			{
				Monster->ArachnidJumpOn();
			}
			else
			{
				Monster->ArachnidJumpOff();
			}
		}
	}
}
