// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Maynard/MobMaynard.h"
#include "Components/CapsuleComponent.h"

AMobMaynard::AMobMaynard()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("RightHand"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("RightHand"));

	MaynardType = FMath::RandRange(0, 1);
}

int32 AMobMaynard::GetMaynardType()
{
	return MaynardType;
}
