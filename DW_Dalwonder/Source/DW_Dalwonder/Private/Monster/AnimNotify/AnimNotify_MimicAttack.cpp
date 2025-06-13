// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_MimicAttack.h"

#include "Monster/NormalMonster/Mimic/DW_Mimic.h"

void UAnimNotify_MimicAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_Mimic* Monster = Cast<ADW_Mimic>(Owner);

		if (IsValid(Monster))
		{
			Monster->MimicAttack();
		}
	}
}
