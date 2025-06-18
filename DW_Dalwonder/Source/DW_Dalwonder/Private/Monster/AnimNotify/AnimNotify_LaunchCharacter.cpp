// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_LaunchCharacter.h"
#include "Monster/DW_MonsterBase.h"

void UAnimNotify_LaunchCharacter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(Owner);

		FVector LaunchVector = Monster->GetActorForwardVector();
		LaunchVector.X *= LaunchX;
		LaunchVector.Y += LaunchY;
		LaunchVector.Z += LaunchZ;

		if (IsValid(Monster))
		{
			Monster->LaunchCharacter(LaunchVector, true, true);
		}
	}
}
