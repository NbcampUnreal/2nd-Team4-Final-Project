// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_NormalMonsterBase.h"


// Sets default values
ADW_NormalMonsterBase::ADW_NormalMonsterBase(): bIsAlerted(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADW_NormalMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADW_NormalMonsterBase::AlertNearbyMonsters(int32 AlertDistance)
{
	//근처 몬스터들에게 FoundPlayer(), SetAlerted(true) 호출
}

void ADW_NormalMonsterBase::FoundPlayer()
{
	//플레이어를 추적하도록 함
}

void ADW_NormalMonsterBase::SetAlerted(bool AlertValue)
{
	bIsAlerted = AlertValue;
}
