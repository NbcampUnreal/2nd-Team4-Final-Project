// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/eye/MobEye.h"
#include "Components/CapsuleComponent.h"

AMobEye::AMobEye()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

float AMobEye::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0;
	
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (MonsterHP <= 0)
	{
		bIsDead = true;
		EyeDeadLogic();
	}

	return 0;
}

void AMobEye::Dead()
{
	
}

void AMobEye::DeadLogic()
{
	Super::DeadLogic();
}
