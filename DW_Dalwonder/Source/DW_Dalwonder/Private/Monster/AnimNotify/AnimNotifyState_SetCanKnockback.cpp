// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotifyState_SetCanKnockback.h"

#include "Monster/DW_MonsterBase.h"

void UAnimNotifyState_SetCanKnockback::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(Owner);

		if (IsValid(Monster))
		{
			Monster->bCanKnockbackByAttack = true;
		}
	}
}

void UAnimNotifyState_SetCanKnockback::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(Owner);

		if (IsValid(Monster))
		{
			Monster->bCanKnockbackByAttack = false;
		}
	}
}
