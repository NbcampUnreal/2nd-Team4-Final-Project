// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_StartMovement.h"
#include "Monster/DW_MonsterBase.h"

void UAnimNotify_StartMovement::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(Owner);

		if (IsValid(Monster))
		{
		}
	}
}
