// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_VisibleSwitch.h"
#include "Monster/DW_MonsterBase.h"

void UAnimNotify_VisibleSwitch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(Owner))
		{
			Monster->GetMesh()->SetVisibility(bSetVisibility);
		}
	}
}
