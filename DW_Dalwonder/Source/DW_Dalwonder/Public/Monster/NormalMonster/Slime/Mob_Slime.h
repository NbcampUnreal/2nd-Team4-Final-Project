// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "Mob_Slime.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API AMob_Slime : public ADW_NormalMonsterBase
{
	GENERATED_BODY()
	
protected:
	AMob_Slime();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	void Dead() override;

public:
	UFUNCTION(BlueprintCallable)
	void LaunchActor(float Strength, float UpwardRatio, bool bIsForward, FVector TargetLocation);

	UFUNCTION(BlueprintCallable)
	void Attack_Explose();

protected:
	void LaunchSelfFromHit(AActor* DamageCauser);

	void UpdateMaterialFromHit();
	void MaterialUpdater();

	void DelayDead();
	void DeadUpdater();

public:
	// 스폰시킬 TelegraphActor 블루프린트 클래스입니다.
	UPROPERTY(EditAnywhere, Category = "Telegraph")
	TSubclassOf<AActor> TelegraphActorClass;

	UPROPERTY(EditAnywhere, Category = "Value")
	int32 HitMaterialUpdateTime = 15;

private:
	class USkeletalMeshComponent* Mesh;

	float BandIntensity;
	float BandSpeed;
	float InteractiveIntensity;

	float AfterBandIntensity;
	float AfterBandSpeed;
	float AfterInteractiveIntensity;

	float DeadZOffset;
	float CurrentZOffset;

	FVector CurrentActorLocation;

	FTimerHandle MaterialUpdateTimerHandle;
	FTimerHandle DeadUpdateTimerHandle;

	int32 HitMaterialUpdateCount = 0;
	int32 DeadUpdateCount = 0;

	/*FVector MeshZOffset;

	FVector InitialScale = FVector(2.f, 2.f, 2.f);
	FVector TargetScale;
	FVector StartScale;

	float ScaleDuration = 0.1f;
	float ScaleElapsed = 0.f;
	bool bIsScaling = false;

	bool bIsLandingBounce = false;
	bool bShouldBounceMesh = false;*/
};
