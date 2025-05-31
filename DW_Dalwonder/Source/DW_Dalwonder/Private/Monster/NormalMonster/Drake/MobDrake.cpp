// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Drake/MobDrake.h"
#include "Components/CapsuleComponent.h"

AMobDrake::AMobDrake()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("LeftHand"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("LeftHand"));

	HeightLocation = GetActorLocation();
	HeightLocation.Z += 400.f;
	SetActorLocation(HeightLocation);
}
