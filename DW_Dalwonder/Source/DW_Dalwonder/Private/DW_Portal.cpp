// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_Portal.h"

#include "DW_GameInstance.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADW_Portal::ADW_Portal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ADW_Portal::Interact_Implementation(AActor* Interactor)
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		GI->LastPortalType = PortalType;
	}

	if (TargetLevel.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetLevel not valid, trying to load: %s"), *TargetLevel.ToSoftObjectPath().ToString());
		TargetLevel.LoadSynchronous();
	}

	if (TargetLevel.IsValid())
	{
		FString LevelNameString = TargetLevel.GetAssetName();
		UGameplayStatics::OpenLevel(this, FName(*LevelNameString));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TargetLevel could not be loaded or is invalid"));
	}
}

// Called when the game starts or when spawned
void ADW_Portal::BeginPlay()
{
	Super::BeginPlay();
	
}
