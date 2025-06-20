// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Character/DW_CharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectileBase::AProjectileBase() : HitEffectSize(1.f), DestroyDelay(10.f), CollisionRadius(1.f)
{
 	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(10.f);
	CollisionComponent->SetCollisionProfileName("ProjectileActor");
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnProjectileHit);
	RootComponent = CollisionComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(CollisionComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->Velocity = FVector(1.f, 0, 0);

	Tags.Add(FName("Projectile"));
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag("Projectile") && !OtherActor->ActorHasTag("Monster") && !OtherActor->ActorHasTag("Weapon"))
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (OtherActor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, nullptr, this, nullptr);
			HitEffectSpawnLogic(Hit);
			Destroy();
		}
		else
		{
			GetWorldTimerManager().ClearTimer(DestroyTimer);

			if (bSpawnAtLand)
			{
				HitEffectSpawnLogic(Hit);
			}

			if (!bRemainNiagara)
			{
				Destroy();
			}
			else
			{
				bUseCurvedTrajectory = false;
				GetWorldTimerManager().SetTimer(DestroyTimer, this, &AProjectileBase::DestroyToDelay, DestroyDelay, false);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	}
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AProjectileBase::DestroyToDelay, HitDestroyDelay, false);
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADW_CharacterBase::StaticClass(), FoundActors);

	if (bUseCurvedTrajectory)
	{
		for (AActor* Actor : FoundActors)
		{
			if (ADW_CharacterBase* Character = Cast<ADW_CharacterBase>(Actor))
			{
				if (bUseCurvedTrajectory && Character)
				{
					LaunchCurvedProjectile(Character);
				}
			}
		}
	}

}

void AProjectileBase::OnConstruction(const FTransform& Transform)
{
	CollisionComponent->SetSphereRadius(CollisionRadius);
}

void AProjectileBase::HitEffectSpawnLogic(const FHitResult& Hit)
{
	if (HitEffect)
	{
		FVector NiagaraSpawnLocation;
		NiagaraSpawnLocation = (Hit.Normal * HitEffectOffset) + Hit.Location;

		FVector NiagaraSpawnSize;
		NiagaraSpawnSize.X = HitEffectSize;
		NiagaraSpawnSize.Y = HitEffectSize;
		NiagaraSpawnSize.Z = HitEffectSize;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitEffect,
			NiagaraSpawnLocation,
			Hit.ImpactNormal.Rotation(),
			NiagaraSpawnSize
		);
	}
}

void AProjectileBase::DestroyToDelay()
{
	Destroy();
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bUseCurvedTrajectory)
	{
		ElapsedTime += DeltaTime;

		float Alpha = ElapsedTime / CurveDuration;

		if (Alpha < 1.f)
		{
			FVector FlatPos = FMath::Lerp(CurveStartLocation, CurveTargetLocation, Alpha);
			FVector Offset = CurveDirection * FMath::Sin(Alpha * PI) * CurveHeight;
			FVector NewLocation = FlatPos + Offset;

			LastVelocity = (NewLocation - GetActorLocation()) / DeltaTime;
			SetActorLocation(NewLocation);
		}
		else
		{
			FVector NewLocation = GetActorLocation() + LastVelocity * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}
}

void AProjectileBase::LaunchCurvedProjectile(AActor* Target)
{
	if (!IsValid(Target)) return;
	
	ElapsedTime = 0.f;

	CurveStartLocation = GetActorLocation();
	CurveTargetLocation = Target->GetActorLocation();
	CurveDuration = 1.0f;
	CurveHeight = 300.f;

	// 휘는 방향 계산 (아래로는 X)
	FVector ToTarget = (CurveTargetLocation - CurveStartLocation).GetSafeNormal();
	FVector RandomDir = UKismetMathLibrary::RandomUnitVector();
	RandomDir.Z = 0.f;
	RandomDir.Normalize();
	
	FVector UpComponent = FVector::UpVector * FMath::FRandRange(MinUpBias, MaxUpBias);

	CurveDirection = (RandomDir + UpComponent).GetSafeNormal();
}

