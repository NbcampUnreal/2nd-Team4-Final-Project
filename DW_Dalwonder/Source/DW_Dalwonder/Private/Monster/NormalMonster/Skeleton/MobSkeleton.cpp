// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Skeleton/MobSkeleton.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AI/Navigation/NavigationTypes.h"

AMobSkeleton::AMobSkeleton()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("hand_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("hand_r"));

	bIsStrafe = false;
}

void AMobSkeleton::BeginPlay()
{
	Super::BeginPlay();

	//SetSpawnLocation();

}

void AMobSkeleton::PlayAlertMontage()
{
	Super::PlayAlertMontage();

	GetCharacterMovement()->DisableMovement();
	float MontageDuration = AlertMontage->GetPlayLength();

	if (GetWorld())
	{
		FTimerHandle MovementTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			MovementTimerHandle,   
			this,     
			&AMobSkeleton::SetMovementWalk, 
			MontageDuration, 
			false
		);
	}
}

void AMobSkeleton::UseFirstSkill()
{
	if (IsValid(FirstSkill))
	{
		if (GetMesh())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(FirstSkill);
		}
	}
}

void AMobSkeleton::UseSecondSkill()
{
	if (IsValid(SecondSkill))
	{
		if (GetMesh())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(SecondSkill);
		}
	}
}

void AMobSkeleton::SetMovementWalk()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

FVector AMobSkeleton::GetSpawnLocation()
{
	return SpawnLocation;
}

void AMobSkeleton::SetSpawnLocation()
{
	SpawnLocation = GetActorLocation();
}

bool AMobSkeleton::SetRandomLocations(float RanRadius, float DistanceFromMe)
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSystem)
	{
		FVector AtForward = GetActorLocation() + GetActorForwardVector() * DistanceFromMe;
		FVector AtBackward = GetActorLocation() - GetActorForwardVector() * DistanceFromMe;

		for(int i = 0; i < 10; i++)
		{
			bool bFound1 = false;
			bool bFound2 = false;

			FNavLocation OutLocation1;
			FNavLocation OutLocation2;

			bFound1 = NavSystem->GetRandomReachablePointInRadius(AtForward, RanRadius, OutLocation1);
			bFound2 = NavSystem->GetRandomReachablePointInRadius(AtBackward, RanRadius, OutLocation2);

			if (bFound1 && bFound2)
			{
					UNavigationPath* NavPath1 = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetSpawnLocation(), OutLocation1.Location);
					UNavigationPath* NavPath2 = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), OutLocation1.Location, OutLocation2.Location);

				if (NavPath1 && NavPath1->IsValid() && NavPath1->PathPoints.Num() > 1)
				{
					if (NavPath2 && NavPath2->IsValid() && NavPath2->PathPoints.Num() > 1)
					{
						RandomLocation1 = OutLocation1.Location;
						RandomLocation2 = OutLocation2.Location;

						DrawDebugSphere(GetWorld(), RandomLocation1, 30.f, 12, FColor::Green, false, 5.f);
						DrawDebugSphere(GetWorld(), RandomLocation2, 30.f, 12, FColor::Green, false, 5.f);

						return true;
					}
				}
			}
		}
	}

	return false;
}

FVector AMobSkeleton::GetPatrolPointFront()
{
	return RandomLocation1;
}

FVector AMobSkeleton::GetPatrolPointBack()
{
	return RandomLocation2;
}


