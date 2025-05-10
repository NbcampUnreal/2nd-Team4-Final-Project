// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Sevarog/DW_Sevarog.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ADW_Sevarog::ADW_Sevarog()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//부착 후 에디터에서 위치 세부 조정 필요
	TraceStart->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("weapon_r"));
}

// Called when the game starts or when spawned
void ADW_Sevarog::BeginPlay()
{
	Super::BeginPlay();

	SetMovementSpeed(400);
	SetAccelerationSpeed(200);
}

