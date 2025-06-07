// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Character/DW_CharacterBase.h"

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

