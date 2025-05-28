// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_SpawnProjectile.h"
#include "Projectile/ProjectileSpawnerComponent.h"

void UAnimNotify_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UProjectileSpawnerComponent* Spawner = Owner->FindComponentByClass<UProjectileSpawnerComponent>())
		{
			Spawner->SpawnProjectile(ProjectileIndex);
		}
	}
}
