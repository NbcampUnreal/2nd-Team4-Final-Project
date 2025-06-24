// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotifyState_SetDamageMuliplier.h"

#include "Monster/DW_MonsterBase.h"

void UAnimNotifyState_SetDamageMuliplier::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                      float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(Owner);

		if (IsValid(Monster))
		{
			Monster->SetMonsterDamageMultiplier(DamageMultiplier);
		}
	}
}

void UAnimNotifyState_SetDamageMuliplier::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(Owner);

		if (IsValid(Monster))
		{
			Monster->SetMonsterDamageMultiplier(1.f);
		}
	}
}
