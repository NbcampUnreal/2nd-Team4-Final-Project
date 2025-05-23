// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/UseOtherHandOff.h"
#include "Monster/DW_MonsterBase.h"

void UUseOtherHandOff::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(Owner);

		if (IsValid(Monster))
		{
			Monster->bUseOtherHand = false;
		}
	}
}
