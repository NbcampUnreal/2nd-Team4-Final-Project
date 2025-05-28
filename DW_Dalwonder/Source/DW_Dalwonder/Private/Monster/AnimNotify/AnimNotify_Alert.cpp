// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_Alert.h"

#include "Monster/NormalMonster/DW_NormalMonsterBase.h"

void UAnimNotify_Alert::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_NormalMonsterBase* Monster = Cast<ADW_NormalMonsterBase>(Owner);

		if (IsValid(Monster))
		{
			Monster->AlertNearbyMonsters_Implementation(AlertRange);
		}
	}
}
