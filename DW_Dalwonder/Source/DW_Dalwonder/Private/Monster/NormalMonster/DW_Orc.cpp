// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/DW_Orc.h"


// Sets default values
ADW_Orc::ADW_Orc()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(GetMesh(), TEXT("hand_r")); 
}

// Called when the game starts or when spawned
void ADW_Orc::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADW_Orc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADW_Orc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

