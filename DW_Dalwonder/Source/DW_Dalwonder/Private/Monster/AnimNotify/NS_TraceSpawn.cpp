// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/NS_TraceSpawn.h"
#include "Projectile/ProjectileSpawnerComponent.h"

void UNS_TraceSpawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UProjectileSpawnerComponent* Spawner = Owner->FindComponentByClass<UProjectileSpawnerComponent>())
		{
			Spawner->SpawnConnectWithTarget(ConnectIndex, AddDamage);
		}
	}
}
