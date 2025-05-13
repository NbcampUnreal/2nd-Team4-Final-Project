// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Skeleton/MobSkeleton.h"
#include "Components/CapsuleComponent.h"

AMobSkeleton::AMobSkeleton()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("hand_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("hand_r"));
}

