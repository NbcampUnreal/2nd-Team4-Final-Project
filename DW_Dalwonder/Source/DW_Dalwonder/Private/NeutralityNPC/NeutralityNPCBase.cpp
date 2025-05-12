// Fill out your copyright notice in the Description page of Project Settings.


#include "NeutralityNPC/NeutralityNPCBase.h"
//#include "Kismet/GameplayStatics.h"

// Sets default values
ANeutralityNPCBase::ANeutralityNPCBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ANeutralityNPCBase::OnInteract(AActor* Interactor)
{
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("NeutralityNPCBase -> OnInteract"));
	}*/
}

// Called when the game starts or when spawned
void ANeutralityNPCBase::BeginPlay()
{
	Super::BeginPlay();
	
}

