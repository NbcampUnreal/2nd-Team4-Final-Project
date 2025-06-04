// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Skeleton/MobSkeleton.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AI/Navigation/NavigationTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

AMobSkeleton::AMobSkeleton()
{
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(GetCapsuleComponent());
	NiagaraComponent->SetAutoActivate(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("hand_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("hand_r"));


	bIsStrafe = false;
}

void AMobSkeleton::BeginPlay()
{
	Super::BeginPlay();


	CurrentZ = GetActorLocation().Z;

	//SetSpawnLocation();
	DefaultHP = MonsterMaxHP;
	DefaultDamage = MonsterDamage;

}

void AMobSkeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	float ZOffset = AnimInstance->GetCurveValue(FName("ZOffset"));

	FVector Location = GetActorLocation();
	Location.Z = CurrentZ + ZOffset; 
	SetActorLocation(Location);

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

float AMobSkeleton::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bHaveEnergeSheild)
	{
		DamageAmount /= 2;
	}

	/*if (DamageAmount >= MonsterMaxHP * 0.3f)
	{
		if (AAIController* AICon = Cast<AAIController>(GetController()))
		{
			if (UBlackboardComponent* BBC = AICon->GetBlackboardComponent())
			{
				BBC->SetValueAsBool(FName("bCanBehavior"), false);

				PlayHitMontage();

				GetWorldTimerManager().SetTimer(HitDelayTimer, this, &ADW_NormalMonsterBase::BehaviorOn, HitDelay, false);
			}
		}
	}*/

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	/*if (MonsterHP <= 0)
	{
		GetWorldTimerManager().ClearTimer(HitDelayTimer);
	}*/

    return 0.0f;
}

void AMobSkeleton::Dead()
{
	NiagaraComponent->Deactivate();

	Super::Dead();
}

void AMobSkeleton::AffectToEnergeShield()
{
	if (bHaveEnergeSheild)
	{
		return;
	}

	bHaveEnergeSheild = true;

	if (NiagaraComponent)
	{
		NiagaraComponent->Activate(true);
	}

	GetWorldTimerManager().SetTimer(EnergeShieldTimer, this, &AMobSkeleton::EnergeShieldDeactive, 10.f, false);

}

void AMobSkeleton::EnergeShieldDeactive()
{
	GetWorldTimerManager().ClearTimer(ScaleUpTimer);

	bHaveEnergeSheild = false;

	NiagaraComponent->Deactivate();
}

void AMobSkeleton::AffectToEnhance()
{
	if (bHaveEnhanced)
	{
		return;
	}

	bHaveEnhanced = true;

	MonsterMaxHP += DefaultHP / 2;
	MonsterHP += DefaultHP / 2;

	MonsterDamage += DefaultDamage / 2;
	
	GetWorldTimerManager().SetTimer(ScaleUpTimer, this, &AMobSkeleton::ScaleUp, 0.05f, true);
	GetWorldTimerManager().SetTimer(EnhancedTimer, this, &AMobSkeleton::EnhanceDeactive, 10.f, false);

}

void AMobSkeleton::ScaleUp()
{
	if (ScaleUpCount > 40)
	{
		ScaleUpCount = 40;
		GetWorldTimerManager().ClearTimer(ScaleUpTimer);
	}

	float Alpha = FMath::Clamp(ScaleUpCount / 40.f, 0.f, 1.f);
	float NewScale = FMath::Lerp(1.f, 1.3f, Alpha);

	SetActorScale3D(FVector(NewScale));

	ScaleUpCount++;

}

void AMobSkeleton::ScaleDown()
{
	if (ScaleUpCount < 0)
	{
		ScaleUpCount = 0;
		GetWorldTimerManager().ClearTimer(ScaleDownTimer);
	}

	float Alpha = FMath::Clamp(ScaleUpCount / 40.f, 0.f, 1.f);
	float NewScale = FMath::Lerp(1.f, 1.3f, Alpha);

	SetActorScale3D(FVector(NewScale));

	ScaleUpCount--;

}

void AMobSkeleton::EnhanceDeactive()
{
	GetWorldTimerManager().SetTimer(ScaleDownTimer, this, &AMobSkeleton::ScaleDown, 0.05f, true);

	bHaveEnhanced = false;

	MonsterMaxHP = DefaultHP;
	if (MonsterHP > DefaultHP)
	{
		MonsterHP = DefaultHP;
	}

	MonsterDamage = DefaultDamage;
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


