// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/AISpawner.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster/NormalMonster/AIOptimizer.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
//#include "NavigationSystem.h"

AAISpawner::AAISpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComponent);
	SpawnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SpawnCollision->SetupAttachment(SceneComponent);
	DetectCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	DetectCollision->SetupAttachment(SceneComponent);

	SpawnDataTable = nullptr;
	SpawnHeight = 100.f;

	DetectRadius = 2000.f;
	SpawnRadius = 800.f;

}

void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnDelay == 0.f)
	{
		InitSpawner();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(InitSpawnTimer, this, &AAISpawner::InitSpawner, SpawnDelay, false);

	}
}

void AAISpawner::OnConstruction(const FTransform& Transform)
{
	DetectCollision->SetSphereRadius(DetectRadius);
	SpawnCollision->SetBoxExtent(FVector(SpawnRadius, SpawnRadius, SpawnRadius));
}

void AAISpawner::SpawnerON()
{
	InitSpawnByRadius();
}

bool AAISpawner::bIsPlayerInRadius()
{
	float ClosetPlayerDistanceSquared = FLT_MAX;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (!IsValid(PlayerCharacter))
		return false;

	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	float CurrentPlayerDistanceSquared = UKismetMathLibrary::Vector_DistanceSquared(DetectCollision->GetComponentLocation(), PlayerLocation);

	if (CurrentPlayerDistanceSquared < ClosetPlayerDistanceSquared)
	{
		ClosetPlayerDistanceSquared = CurrentPlayerDistanceSquared;
	}

	if (DetectRadius * DetectRadius >= ClosetPlayerDistanceSquared)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Monster Spawner : Player Detected!"));
		}

		return true;
	}

	return false;
}

bool AAISpawner::bCanSpawnActor()
{
	/*if (플레이어가 마을을 가거나 매우 멀어지면 스폰 가능으로 변경)
	{
		return true;
	}

	return false;*/

	return true;
}

void AAISpawner::InitSpawner()
{
	GetWorld()->GetTimerManager().ClearTimer(InitSpawnTimer);

	if (SpawnDataTable == nullptr) return;

	InitSpawnByRadius();

}

void AAISpawner::InitSpawnByRadius()
{
	GetWorld()->GetTimerManager().SetTimer(CheckRadiusTimer, this, &AAISpawner::CheckRadius, 0.9f, true);
}

void AAISpawner::SpawnMonstersFromDataTable()
{
	if (!GetWorld()) return;
	if (!SpawnDataTable) return;

	TArray<FAISpawnRow*> Rows;
	const FString ContextString(TEXT("AISpawnerContext"));
	SpawnDataTable->GetAllRows(ContextString, Rows);

	/*static const FString ContextString(TEXT("Monster Spawn Context"));
	TArray<FName> RowNames = SpawnDataTable->GetRowNames();*/

	for (FAISpawnRow* RowName : Rows)
	{
		if (RowName && RowName->SpawnClass && RowName->Amount)
		{
			for (int32 i = 0; i < RowName->Amount; ++i)
			{
				if (UClass* SpawningClass = RowName->SpawnClass.Get())
				{
					FTransform SpawnLocation = GetRandomSpawnPoint();
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

					if (SpawningClass == nullptr) return;

					GetWorld()->SpawnActor<AActor>(SpawningClass, SpawnLocation, SpawnParams);
					
					
				}

			}
		}
	}
}

void AAISpawner::TryToSpawnGroup()
{
	if (bCanSpawnActor())
	{
		SpawnMonstersFromDataTable();
	}
}

void AAISpawner::CheckRadius()
{
	if (bIsPlayerInRadius())
	{
		GetWorld()->GetTimerManager().ClearTimer(CheckRadiusTimer);
		TryToSpawnGroup();
	}
}

FTransform AAISpawner::GetRandomSpawnPoint()
{
	FVector BoxExtent = SpawnCollision->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnCollision->GetComponentLocation();

	FVector PointLocation = BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		SpawnHeight
	);

	FRotator PointLotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

	return FTransform(PointLotation, PointLocation, FVector(1.f, 1.f, 1.f));
}
