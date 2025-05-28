// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monster/NormalMonster/SpawnType.h"
#include "Interface/BearableInterface.h"
#include "AISpawner.generated.h"

class UBoxComponent;
class USphereComponent;
class DW_NormalMonsterBase;

UCLASS()
class DW_DALWONDER_API AAISpawner : public AActor, public IBearableInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AAISpawner();

	// 몬스터 스포너를 다시 작동시키는 함수
	UFUNCTION(BlueprintCallable)
	void SpawnerON();

	virtual bool CanBeCut_Implementation(const FHitResult& Hit) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	bool bIsPlayerInRadius();
	bool bCanSpawnActor();

	// Spawn Logic
	// 스폰 로직
protected:
	void InitSpawner();
	void InitSpawnByRadius();

	FTransform GetRandomSpawnPoint();
	void TryToSpawnGroup();

	void SpawnMonstersFromDataTable();

	void CheckRadius();


public:
	// The class of the Actor to spawn
	// 스폰시킬 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Property")
	UDataTable* SpawnDataTable;
	// Default spawn height(Z offset)
	// 기본 스폰 높이(높이 공간)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Property")
	float SpawnHeight;

	// Delay when executing new spawn commands
	// 새로운 스폰 명령 실행시 딜레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Property")
	float SpawnDelay;

	// Range to detect player character
	// 플레이어를 감지하는 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn On Radius")
	float DetectRadius;
	// Range to spawn AI character
	// AI 캐릭터가 랜덤하게 스폰되는 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn On Radius")
	float SpawnRadius;

protected:
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* SpawnCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* DetectCollision;

	FTimerHandle InitSpawnTimer;
	FTimerHandle SpawnLoopTimer;
	FTimerHandle CheckRadiusTimer;
};
