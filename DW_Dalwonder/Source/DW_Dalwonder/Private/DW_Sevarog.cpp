// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_Sevarog.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ADW_Sevarog::ADW_Sevarog()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADW_Sevarog::BeginPlay()
{
	Super::BeginPlay();

	SetMovementSpeed(400);
	SetAccelerationSpeed(200);
}

void ADW_Sevarog::SetMovementSpeed(int32 NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ADW_Sevarog::SetAccelerationSpeed(int32 NewAccelerationSpeed)
{
	GetCharacterMovement()->MaxAcceleration = NewAccelerationSpeed;
}
