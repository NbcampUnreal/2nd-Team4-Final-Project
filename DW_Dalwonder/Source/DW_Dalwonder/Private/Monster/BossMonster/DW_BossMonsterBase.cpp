// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/DW_BossMonsterBase.h"
#include "Components/AudioComponent.h"


// Sets default values
ADW_BossMonsterBase::ADW_BossMonsterBase(): CurrentPhase(0), BGM(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BGMSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGM"));
	BGMSoundComponent->SetupAttachment(RootComponent);
	BGMSoundComponent->bAutoActivate = false;

	Tags.Add(TEXT("BossMonster"));
}

// Called when the game starts or when spawned
void ADW_BossMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

int32 ADW_BossMonsterBase::GetCurrentPhase()
{
	return CurrentPhase;
}

void ADW_BossMonsterBase::SetCurrentPhase(int32 NewPhase)
{
	CurrentPhase = NewPhase;
}

// void ADW_BossMonsterBase::SetPhaseBlackboardValues(int32 NewPhase)
// {
// 	//Blackboard의 값을 바꿀 예정
// }

void ADW_BossMonsterBase::SetBGM(USoundBase* NewBGM)
{
	//BGM 변경로직
}
