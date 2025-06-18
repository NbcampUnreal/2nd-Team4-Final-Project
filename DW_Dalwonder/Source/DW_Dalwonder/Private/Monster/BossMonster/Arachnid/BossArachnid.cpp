// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Arachnid/BossArachnid.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/DW_CharacterBase.h"

ABossArachnid::ABossArachnid()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("hand_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("hand_r"));
}

void ABossArachnid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPlayerCharacter())
	{
		FRotator CurrentRot = GetActorRotation();
		FVector TargetLocation = GetPlayerCharacter()->GetActorLocation();
		FVector MyLocation = GetActorLocation();

		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);

		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);

		SetActorRotation(NewRot);
	}
}
