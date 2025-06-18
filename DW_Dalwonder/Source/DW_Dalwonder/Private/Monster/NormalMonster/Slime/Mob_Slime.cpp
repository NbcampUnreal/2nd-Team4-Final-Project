// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Slime/Mob_Slime.h"
#include "Components/CapsuleComponent.h"
#include "Telegraph/TelegraghActor.h"

AMob_Slime::AMob_Slime()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = GetMesh();

	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");

}

void AMob_Slime::BeginPlay()
{
	Super::BeginPlay();

	//MeshZOffset = Mesh->GetRelativeLocation();

}

void AMob_Slime::Tick(float DeltaTime)
{
	/*Super::Tick(DeltaTime);

	if (bIsScaling)
	{
		ScaleElapsed += DeltaTime;
		float Alpha = FMath::Clamp(ScaleElapsed / ScaleDuration, 0.f, 1.f);
		FVector NewScale = FMath::Lerp(StartScale, TargetScale, Alpha);
		Mesh->SetRelativeScale3D(NewScale);

		if (bShouldBounceMesh)
		{
			if (Alpha <= 0.7f)
			{
				float ZOffset = FMath::Lerp(MeshZOffset.Z, MeshZOffset.Z - 20.f, Alpha);
				Mesh->SetRelativeLocation(FVector(0.f, 0.f, ZOffset));
			}
			else
			{
				float ZOffset = FMath::Lerp(MeshZOffset.Z - 20.f, MeshZOffset.Z, Alpha);
				Mesh->SetRelativeLocation(FVector(0.f, 0.f, ZOffset));
			}
		}

		if (Alpha >= 1.f)
		{
			if (bIsLandingBounce)
			{
				StartScale = TargetScale;
				TargetScale = InitialScale;
				ScaleElapsed = 0.f;
				bIsLandingBounce = false; 
				bShouldBounceMesh = true;
			}
			else
			{
				bIsScaling = false;
				bShouldBounceMesh = false;

				Mesh->SetRelativeLocation(MeshZOffset);
			}
		}
	}*/
}

void AMob_Slime::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	Attack_Explose();

	/*StartScale = Mesh->GetRelativeScale3D();
	TargetScale = InitialScale * 1.2f;    
	ScaleElapsed = 0.f;
	bIsScaling = true;
	bIsLandingBounce = true;*/
}

void AMob_Slime::LaunchActor(float Strength, float UpwardRatio, bool bIsForward, FVector TargetLocation)
{
		FVector Forward = (TargetLocation - GetActorLocation()).GetSafeNormal();
		FVector Up = FVector::UpVector;

		if (!bIsForward)
		{
			Forward *= -1.f;
		}

		FVector LaunchDir = (Forward + Up * UpwardRatio).GetSafeNormal();
		FVector LaunchVelocity = LaunchDir * Strength;

		LaunchCharacter(LaunchVelocity, true, true);

		/*StartScale = InitialScale;
		TargetScale = FVector(0.7f, 0.7f, 2.5f);
		ScaleElapsed = 0.f;
		bIsScaling = true;
		bIsLandingBounce = false;*/
}

void AMob_Slime::Attack_Explose()
{
	FVector SpawnLocation = GetActorLocation();

		FVector Start = GetActorLocation();
		FVector End = Start - FVector(0.f, 0.f, 1000.f);

		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("TelegraphTrace")), false, this);
		TraceParams.bReturnPhysicalMaterial = false;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
		{
			SpawnLocation = HitResult.Location + FVector(0.f, 0.f, 10.f);
		}
		else
		{
			SpawnLocation = GetActorLocation();
		}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ATelegraghActor>(
		TelegraphActorClass,
		SpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);
}
