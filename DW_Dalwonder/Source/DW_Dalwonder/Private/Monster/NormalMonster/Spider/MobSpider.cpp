// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Spider/MobSpider.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMobSpider::AMobSpider()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("foot_l"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("foot_r"));
}

void AMobSpider::DeadLogic()
{
	SetActorTickEnabled(false);
	GetCharacterMovement()->DisableMovement();

	FTimerHandle DestroyDelayTimer;
	GetWorldTimerManager().SetTimer(DestroyDelayTimer, this, &ADW_NormalMonsterBase::DestroyDissolve, DestroyTime, false);

}
