// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_SevarogAirAttack.h"

#include "Monster/BossMonster/Sevarog/DW_Sevarog.h"

void UAnimNotify_SevarogAirAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_Sevarog* Monster = Cast<ADW_Sevarog>(Owner);

		if (IsValid(Monster))
		{
			Monster->AirAttack();
		}
	}
}
