#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "DW_WoodMonsterBase.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_WoodMonsterBase : public ADW_NormalMonsterBase
{
	GENERATED_BODY()

public:
	ADW_WoodMonsterBase();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

public:
	UFUNCTION(BlueprintCallable)
	void SpawnToWoodGolem();

private:
	FTransform GetRandomSpawnPoint();

private:

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* SpawnCollision;

	// 골렘을 소환하기 위한 기준 체력 백분률입니다. 데미지를 입었을 때 해당 체력 이하라면 자동으로 우드골렘을 스폰합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnHP;

	// 소환시킬 골렘의 수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 SpawnAmount;
	// 골렘의 수를 랜덤으로 정합니다. True일 경우 최소 1마리부터 최대 SpawnAmount로 지정한 수까지 랜덤 범위입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool bUseRandomAmount;

private:
	bool bIsSpawned = false;
};
