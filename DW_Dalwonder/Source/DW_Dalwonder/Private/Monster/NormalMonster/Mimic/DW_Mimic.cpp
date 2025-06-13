// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Mimic/DW_Mimic.h"


// Sets default values
ADW_Mimic::ADW_Mimic()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADW_Mimic::Interact_Implementation(AActor* Interactor)
{
	UE_LOG(LogTemp, Display, TEXT("Interacting %s"), *GetName());
}