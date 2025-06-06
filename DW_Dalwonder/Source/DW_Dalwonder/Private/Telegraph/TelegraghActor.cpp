// Fill out your copyright notice in the Description page of Project Settings.


#include "Telegraph/TelegraghActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"
#include "Character/DW_CharacterBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATelegraghActor::ATelegraghActor()
{
 	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(500.f);
	CollisionComponent->SetCollisionProfileName("NoCollision");
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	//CollisionComponent->OnComponentHit.AddDynamic(this, &ATelegraghActor::OnEffectHit);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATelegraghActor::OnEffectOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATelegraghActor::OnEffectEndOverlap);
	RootComponent = CollisionComponent;

	EffectComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	EffectComponent->SetupAttachment(CollisionComponent);

	Tags.Add(FName("Projectile"));
}

void ATelegraghActor::OnEffectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (!bIsDOT)
		{
			CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, nullptr, this, nullptr);
		}
		else
		{
			if (DOTTargetActor == nullptr)
			{
				DOTTargetActor = OtherActor;

				GetWorldTimerManager().SetTimer(DOTIntervalTimerHandle, this, &ATelegraghActor::DOTIntervalLogic, DOTInterval, true);
			}
		}
	}
}

void ATelegraghActor::OnEffectEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == DOTTargetActor)
	{
		GetWorldTimerManager().ClearTimer(DOTIntervalTimerHandle);
		DOTTargetActor = nullptr;
	}
}

void ATelegraghActor::BeginPlay()
{
	Super::BeginPlay();
	
	if(bUseTelegraph)
	{
		if (TelegraphEffect)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				TelegraphEffect,
				EffectComponent->GetComponentLocation(),
				EffectComponent->GetComponentRotation(),
				EffectComponent->GetComponentScale()
			);
			NiagaraComp->SetVariableFloat(FName("LifeTime"), TelegraphTime);
			NiagaraComp->SetVariableFloat(FName("Scale"), TelegraphScale);
			NiagaraComp->SetVariableLinearColor(FName("Color"), TelegraphColor);
		}

		GetWorldTimerManager().SetTimer(TelegraphTimerHandle, this, &ATelegraghActor::RealEffectSpawnLogic, TelegraphTime, false);
	}
	else
	{
		RealEffectSpawnLogic();
	}
}

void ATelegraghActor::OnConstruction(const FTransform& Transform)
{
	CollisionComponent->SetSphereRadius(CollisionRadius);
}

void ATelegraghActor::RealEffectSpawnLogic()
{
	if (RealEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			RealEffect,
			EffectComponent->GetComponentLocation(),
			EffectComponent->GetComponentRotation(),
			EffectComponent->GetComponentScale()
		);

		CollisionComponent->SetCollisionProfileName("TelegraphField");
	}

	if (bIsDOT)
	{
		DOTDamage = DamageAmount / (DOTDuration / DOTInterval);

		GetWorldTimerManager().SetTimer(DOTDurationTimerHandle, this, &ATelegraghActor::DOTDurationEndLogic, DOTDuration, false);

	}
	else
	{
		GetWorldTimerManager().SetTimer(TelegraphTimerHandle, this, &ATelegraghActor::DestroyToDelay, 3.f, false);

	}
}

void ATelegraghActor::DestroyToDelay()
{
	Destroy();
}

void ATelegraghActor::DOTIntervalLogic()
{
	if (DOTTargetActor && DOTTargetActor->ActorHasTag("Player"))
	{		
		UGameplayStatics::ApplyDamage(DOTTargetActor, DOTDamage, nullptr, this, nullptr);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DOTIntervalTimerHandle);
		DOTTargetActor = nullptr;
	}
}

void ATelegraghActor::DOTDurationEndLogic()
{
	GetWorldTimerManager().ClearTimer(DOTIntervalTimerHandle);
	GetWorldTimerManager().ClearTimer(DOTDurationTimerHandle);

	GetWorldTimerManager().SetTimer(TelegraphTimerHandle, this, &ATelegraghActor::DestroyToDelay, 3.f, false);

}

