// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_MonsterBase.h"


// Sets default values
ADW_MonsterBase::ADW_MonsterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADW_MonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADW_MonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADW_MonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

EMonsterState ADW_MonsterBase::GetCurrentState() const
{
	return CurrentState;
}

void ADW_MonsterBase::SetCurrentState(EMonsterState MonsterState)
{
	CurrentState = MonsterState;
}

