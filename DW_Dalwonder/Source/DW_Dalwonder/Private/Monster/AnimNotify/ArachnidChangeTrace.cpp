// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/ArachnidChangeTrace.h"
#include "Monster/BossMonster/Arachnid/BossArachnid.h"

void UArachnidChangeTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ABossArachnid* Monster = Cast<ABossArachnid>(Owner);

		if (IsValid(Monster))
		{
			Monster->ChangingAttackTrace(Value);
		}
	}
}
