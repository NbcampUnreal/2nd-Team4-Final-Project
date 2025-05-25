// Fill out your copyright notice in the Description page of Project Settings.


#include "Maps/DW_GeometryCollectionActor.h"

#include "GeometryCollection/GeometryCollectionComponent.h"


// Sets default values
ADW_GeometryCollectionActor::ADW_GeometryCollectionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	if (UGeometryCollectionComponent* GCComponent = GetGeometryCollectionComponent())
	{
		GCComponent->SetSimulatePhysics(false);
		GCComponent->SetEnableGravity(false);
		GCComponent->SetCollisionProfileName("NoCollision");
		GCComponent->SetCollisionObjectType(ECC_WorldDynamic);
	}
}

void ADW_GeometryCollectionActor::BeginPlay()
{
	Super::BeginPlay();

}

void ADW_GeometryCollectionActor::DestructActor()
{
	if (UGeometryCollectionComponent* GCComponent = GetGeometryCollectionComponent())
	{
		GCComponent->SetEnableGravity(true); // 중력 다시 켜줌
		GCComponent->SetSimulatePhysics(true);
		GCComponent->SetNotifyRigidBodyCollision(true);
		GCComponent->SetCollisionProfileName("OverlapOnlyPawn");
		GCComponent->SetCollisionObjectType(ECC_WorldDynamic);

		FVector const LaunchDirection = FVector(VectorX, VectorY, VectorZ).GetSafeNormal();

		GCComponent->AddImpulse(LaunchDirection * ImpulseStrength, NAME_None, true);
		// 일정 시간 후 제거
		// GetWorldTimerManager().SetTimer(
		// 	DestroyTimerHandle,
		// 	this,
		// 	&ADW_GeometryCollectionActor::DestroyActor,
		// 	5.0f,
		// 	false
		// );
	}
}

void ADW_GeometryCollectionActor::DestroyActor()
{
	Destroy();
}
