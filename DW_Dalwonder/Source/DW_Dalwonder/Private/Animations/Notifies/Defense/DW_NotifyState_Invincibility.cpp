// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notifies/Defense/DW_NotifyState_Invincibility.h"
#include "DW_CharacterBase.h"

void UDW_NotifyState_Invincibility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ADW_CharacterBase* Char = Cast<ADW_CharacterBase>(MeshComp->GetOwner()))
	{
		Char->SetInvincible(true);
	}
}

void UDW_NotifyState_Invincibility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ADW_CharacterBase* Char = Cast<ADW_CharacterBase>(MeshComp->GetOwner()))
	{
		Char->SetInvincible(false);
	}
}