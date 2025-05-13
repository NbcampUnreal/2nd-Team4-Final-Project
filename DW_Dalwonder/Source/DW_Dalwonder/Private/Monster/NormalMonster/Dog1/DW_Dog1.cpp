// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Dog1/DW_Dog1.h"


// Sets default values
ADW_Dog1::ADW_Dog1()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TraceStart->SetupAttachment(GetMesh(), TEXT("bip01_r_hand"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("bip01_l_hand"));
}

// Called when the game starts or when spawned
void ADW_Dog1::BeginPlay()
{
	Super::BeginPlay();
	
}
