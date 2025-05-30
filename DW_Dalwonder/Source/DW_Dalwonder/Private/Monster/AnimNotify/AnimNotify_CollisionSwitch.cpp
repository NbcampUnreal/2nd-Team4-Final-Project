// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_CollisionSwitch.h"
#include "Components/CapsuleComponent.h"
#include "Monster/DW_MonsterBase.h"

void UAnimNotify_CollisionSwitch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	ADW_MonsterBase* OwnerCharacter = Cast<ADW_MonsterBase>(MeshComp->GetOwner());
	if (!OwnerCharacter) return;

	UCapsuleComponent* CapsuleComp = OwnerCharacter->GetCapsuleComponent();
	if (!CapsuleComp) return;

	if (Preset_NoCollision)
	{
		MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
		CapsuleComp->SetCollisionProfileName(TEXT("NoCollision"));
	}
	else if (Preset_Monster)
	{
		MeshComp->SetCollisionProfileName(TEXT("MonsterMesh"));
		CapsuleComp->SetCollisionProfileName(TEXT("MonsterCapsule"));
	}
	else if (Preset_Pawn)
	{
		MeshComp->SetCollisionProfileName(TEXT("Pawn"));
		CapsuleComp->SetCollisionProfileName(TEXT("Pawn"));
	}
}
