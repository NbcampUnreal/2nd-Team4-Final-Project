// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/WoodMonster/Mob_WoodGolem.h"
#include "Components/CapsuleComponent.h"

AMob_WoodGolem::AMob_WoodGolem()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("hand_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("hand_r"));
}

void AMob_WoodGolem::UseFirstSkill()
{
	if (IsValid(FirstSkill))
	{
		if (GetMesh())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(FirstSkill);
		}
	}
}

void AMob_WoodGolem::BackToNature()
{
	Destroy();
}
