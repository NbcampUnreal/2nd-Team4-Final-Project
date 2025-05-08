// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_NormalMonsterBase.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ADW_NormalMonsterBase::ADW_NormalMonsterBase(): bIsAlerted(false), MonsterAlertDistance(0)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Tags.Add(TEXT("NormalMonster"));
}

// Called when the game starts or when spawned
void ADW_NormalMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADW_NormalMonsterBase::AlertNearbyMonsters_Implementation(const int32 AlertDistance)
{

	// 주변 몬스터들에게 알리는 PerformAttack 시행하게 될 것
	
	TArray<AActor*> OutActors;
	
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		AlertDistance,
		{ UEngineTypes::ConvertToObjectType(ECC_Pawn) },
		nullptr,
		{ this }, // 자기 자신 제외
		OutActors
	);

	for (AActor* Actor : OutActors)
	{
		if (Actor->ActorHasTag("Monster") && Actor->Implements<UDW_NormalMonsterBaseInterface>())
		{
			Execute_SetAlerted(Actor, true);
			Execute_FoundPlayer(Actor);
		}
	}
}

void ADW_NormalMonsterBase::FoundPlayer_Implementation()
{
	if (!bIsAlerted)
	{
		AlertNearbyMonsters(MonsterAlertDistance);
		//플레이어를 추적하도록 함
	}
}

void ADW_NormalMonsterBase::SetAlerted_Implementation(const bool AlertValue)
{
	bIsAlerted = AlertValue;
}
