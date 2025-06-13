// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_Teleport.h"

// Sets default values
ADW_Teleport::ADW_Teleport()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADW_Teleport::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADW_Teleport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

