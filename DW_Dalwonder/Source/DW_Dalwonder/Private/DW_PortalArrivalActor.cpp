// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_PortalArrivalActor.h"

#include "NiagaraFunctionLibrary.h"


// Sets default values
ADW_PortalArrivalActor::ADW_PortalArrivalActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADW_PortalArrivalActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(SpawnNS))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		SpawnNS,
		GetActorLocation(),
		GetActorRotation(),
		FVector(1.f),
		true,
		true);
	}
}
