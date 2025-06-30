// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Slime/Mob_Slime.h"
#include "Components/CapsuleComponent.h"
#include "Telegraph/TelegraghActor.h"
#include "TimerManager.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	UMaterialInstance* BaseMat = Cast<UMaterialInstance>(GetMesh()->GetMaterial(0));
	float OutValue = 0.f;
	if (BaseMat && BaseMat->GetScalarParameterValue(FMaterialParameterInfo("BandIntensity"), OutValue))
	{
		BandIntensity = OutValue;
	}
	if (BaseMat && BaseMat->GetScalarParameterValue(FMaterialParameterInfo("BandSpeed"), OutValue))
	{
		BandSpeed = OutValue;
	}
	if (BaseMat && BaseMat->GetScalarParameterValue(FMaterialParameterInfo("InteractiveIntensity"), OutValue))
	{
		InteractiveIntensity = OutValue;
	}

	AfterBandIntensity = BandIntensity * 2.f;
	AfterBandSpeed = BandSpeed * 5.f;
	AfterInteractiveIntensity = 0.f;

	UMaterialInstanceDynamic* DynMat = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);

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

float AMob_Slime::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	LaunchSelfFromHit(DamageCauser);

	if (MonsterHP - DamageAmount > 0)
	{
		UpdateMaterialFromHit();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(MaterialUpdateTimerHandle);
	}

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	

	return 0.0f;
}

void AMob_Slime::Dead()
{
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = AICon->GetBlackboardComponent())
		{
			BBC->SetValueAsBool(FName("bCanBehaviorr"), false);
		}
	}

	UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(0));
	if (DynMat)
	{
		DynMat->SetScalarParameterValue(FName("BandIntensity"), 0.f);
		DynMat->SetScalarParameterValue(FName("BandSpeed"), 0.f);
	}

	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(DeadUpdateTimerHandle, this, &AMob_Slime::DelayDead, 3.f, false);
	}
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

void AMob_Slime::LaunchSelfFromHit(AActor* DamageCauser)
{
	if (!DamageCauser) return;

	FVector HitDirection = GetActorLocation() - DamageCauser->GetActorLocation();
	HitDirection.Z = 0.f; 
	HitDirection.Normalize();

	FVector LaunchVelocity = HitDirection * 500.f + FVector::UpVector * 300.f;
	LaunchCharacter(LaunchVelocity, true, true);
}

void AMob_Slime::UpdateMaterialFromHit()
{
	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(MaterialUpdateTimerHandle, this, &AMob_Slime::MaterialUpdater, 0.1f, true);
	}
}

void AMob_Slime::MaterialUpdater()
{
	if (HitMaterialUpdateCount > HitMaterialUpdateTime)
	{
		GetWorldTimerManager().ClearTimer(MaterialUpdateTimerHandle);
		return;
	}

	float Alpha = HitMaterialUpdateCount / HitMaterialUpdateTime;

	float BandIntensity_Local = FMath::Lerp(AfterBandIntensity, BandIntensity, Alpha);
	float BandSpeed_Local = FMath::Lerp(AfterBandSpeed, BandSpeed, Alpha);
	float InteractiveIntensity_Local = FMath::Lerp(AfterInteractiveIntensity, InteractiveIntensity, Alpha);

	UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(0));
	if (DynMat)
	{
		DynMat->SetScalarParameterValue(FName("BandIntensity"), BandIntensity_Local);
		DynMat->SetScalarParameterValue(FName("BandSpeed"), BandSpeed_Local);
		DynMat->SetScalarParameterValue(FName("InteractiveIntensity"), InteractiveIntensity_Local);
	}

	HitMaterialUpdateCount++;
}

void AMob_Slime::DelayDead()
{
	CurrentActorLocation = Mesh->GetRelativeLocation();

	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(DeadUpdateTimerHandle, this, &AMob_Slime::DeadUpdater, 0.01f, true);
	}
}

void AMob_Slime::DeadUpdater()
{
	if (DeadUpdateCount > 300)
	{
		GetWorldTimerManager().ClearTimer(DeadUpdateTimerHandle);

		Destroy();

		return;
	}

	float Alpha = DeadUpdateCount / 300.f;

	DeadZOffset = FMath::Lerp(CurrentActorLocation.Z, CurrentActorLocation.Z - 200.f, Alpha);

	FVector NewLocation = FVector(CurrentActorLocation.X, CurrentActorLocation.Y, DeadZOffset);
	Mesh->SetRelativeLocation(NewLocation);

	DeadUpdateCount++;
}
