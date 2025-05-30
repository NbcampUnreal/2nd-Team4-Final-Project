// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileSpawnerComponent.h"
#include "Kismet/GameplayStatics.h"     
#include "CollisionQueryParams.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/DamageType.h"
#include "Character/DW_CharacterBase.h"
#include "Monster/DW_MonsterBase.h"

UProjectileSpawnerComponent::UProjectileSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UProjectileSpawnerComponent::SpawnProjectile(int32 Index)
{
	if (!NiagaraSystemArray.IsValidIndex(Index)) return;

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (PlayerPawn)
		{
			FVector SpawnLocation = GetComponentLocation();
			FRotator SpawnRotation = GetComponentRotation();

			FVector ToPlayer = (PlayerPawn->GetActorLocation() - SpawnLocation).GetSafeNormal();
			SpawnRotation = ToPlayer.Rotation();

			AActor* SpawnedActor = World->SpawnActor<AActor>(NiagaraSystemArray[Index], SpawnLocation, SpawnRotation, SpawnParams);

		}
	}
}

void UProjectileSpawnerComponent::SpawnConnectWithTarget(int32 Index, float DamageIncrease)
{
	if (!ConnectNiagaraSystemArray.IsValidIndex(Index)) return;

	UWorld* World = GetWorld();
	if (World)
	{
		FHitResult HitResult;
		FVector Start = GetComponentLocation();
		FVector End = GetComponentLocation();
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			End = PlayerPawn->GetActorLocation();
		}

		FCollisionQueryParams TraceParams(FName(TEXT("MyTraceTag")), true, GetOwner());
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Camera,
			TraceParams
		);

		if (bHit)
		{
			FVector HitLocation = HitResult.Location;

			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				ConnectNiagaraSystemArray[Index],       
				this,					
				NAME_None,        
				FVector(0.f, 0.f, 0.f),    
				FRotator(0.f, 0.f, 0.f), 
				EAttachLocation::KeepRelativeOffset, 
				true         
			);
			NiagaraComp->SetVariableVec3(FName("BeamStartPoint"), GetComponentLocation());
			NiagaraComp->SetVariableVec3(FName("BeamEndPoint"), HitResult.ImpactPoint + HitResult.ImpactNormal);
			NiagaraComp->SetVariableVec3(FName("ImpactNormal"), HitResult.ImpactNormal);

			AActor* HitActor = HitResult.GetActor();
			if (HitActor && HitActor->IsA(ADW_CharacterBase::StaticClass()))
			{
				ADW_MonsterBase* OwnerMonster = Cast<ADW_MonsterBase>(GetOwner());
				if (OwnerMonster)
				{
					float Damage = OwnerMonster->GetMonsterDamage();
					Damage += DamageIncrease;

					UGameplayStatics::ApplyDamage(
						HitActor,
						Damage,
						GetOwner()->GetInstigatorController(),
						GetOwner(),
						UDamageType::StaticClass()
					);
				}
			}
		}
	}
}

