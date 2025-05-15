// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Skeleton/MobSkeleton.h"
#include "Components/CapsuleComponent.h"

AMobSkeleton::AMobSkeleton()
{
	PrimaryActorTick.bCanEverTick = true;

	//GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("hand_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("hand_r"));
}

void AMobSkeleton::BeginPlay()
{
	Super::BeginPlay();


}

void AMobSkeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Tick called. bIsAttacking: %s"), bIsAttacking ? TEXT("true") : TEXT("false"));

	
	UE_LOG(LogTemp, Warning, TEXT("TraceStart: %f"), TraceStart->GetComponentLocation().X);

}

