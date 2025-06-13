// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_Chest_DropItem.h"

#include "DW_Chest.h"

void UAnimNotify_Chest_DropItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_Chest* Monster = Cast<ADW_Chest>(Owner);

		if (IsValid(Monster))
		{
			Monster->DropItem();
		}
	}
}
