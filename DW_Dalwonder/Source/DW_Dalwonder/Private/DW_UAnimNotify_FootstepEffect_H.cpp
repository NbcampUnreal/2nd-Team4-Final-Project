// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_UAnimNotify_FootstepEffect_H.h"

#include "Character/DW_CharacterBase.h"

void UDW_UAnimNotify_FootstepEffect_H::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ADW_CharacterBase* Character = Cast<ADW_CharacterBase>(MeshComp->GetOwner()))
	{
		Character->SpawnFootstepEffect_H(FootSocketName);
	}
}
