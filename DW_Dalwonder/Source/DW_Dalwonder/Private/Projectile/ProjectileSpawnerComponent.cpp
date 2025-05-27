// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileSpawnerComponent.h"

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

		FVector SpawnLocation = GetComponentLocation();
		FRotator SpawnRotation = GetComponentRotation();

		AActor* SpawnedActor = World->SpawnActor<AActor>(NiagaraSystemArray[Index], SpawnLocation, SpawnRotation, SpawnParams);
	}
}

