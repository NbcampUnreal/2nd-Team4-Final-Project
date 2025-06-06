// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_ElephantAttack.h"

#include "Monster/NormalMonster/Elephant/DW_Elephant.h"

void UAnimNotify_ElephantAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_Elephant* Monster = Cast<ADW_Elephant>(Owner);

		if (IsValid(Monster))
		{
			Monster->ElephantAttack();
		}
	}
}
