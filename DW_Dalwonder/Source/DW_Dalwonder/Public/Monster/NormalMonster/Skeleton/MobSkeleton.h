// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "MobSkeleton.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API AMobSkeleton : public ADW_NormalMonsterBase
{
	GENERATED_BODY()
	
private:
	AMobSkeleton();

	virtual void BeginPlay() override;

public:
	virtual void PlayAlertMontage() override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	// ---------- Animation Function ----------
	// Use first skill anim montage.
	UFUNCTION(BlueprintCallable)
	void UseFirstSkill();
	// Use second skill anim montage.
	UFUNCTION(BlueprintCallable)
	void UseSecondSkill();
	// ---------- -------------------- ----------

	// ----------  Function ----------
	// 
	
	// ---------- -------------------- ----------

	// ---------- Stat & Data Function ----------
	// Set charcter movement mode [ Walk ]
	void SetMovementWalk();

	// Cached spawn location.
	UFUNCTION(BlueprintCallable)
	FVector GetSpawnLocation();
	void SetSpawnLocation();

	// Generate two patrol point at another location.
	UFUNCTION(BlueprintCallable)
	bool SetRandomLocations(float RanRadius, float DistanceFromMe);

	UFUNCTION(BlueprintCallable)
	FVector GetPatrolPointFront();
	UFUNCTION(BlueprintCallable)
	FVector GetPatrolPointBack();
	// ---------- -------------------- ----------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* FirstSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* SecondSkill;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Propertys|Patrol")
	bool bUsePatrol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Propertys|Patrol")
	float GeneratDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Propertys|Patrol")
	float GeneratRandomRadius;*/


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsStrafe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float HitDelay;

	FVector SpawnLocation;
	FVector RandomLocation1;
	FVector RandomLocation2;

	FTimerHandle HitDelayTimer;
};
