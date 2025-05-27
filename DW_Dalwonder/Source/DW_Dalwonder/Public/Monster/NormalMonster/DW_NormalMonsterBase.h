// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_NormalMonsterBaseInterface.h"
#include "Monster/DW_MonsterBase.h"
#include "DW_NormalMonsterBase.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_NormalMonsterBase : public ADW_MonsterBase, public IDW_NormalMonsterBaseInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_NormalMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="NormalMonster")
	bool bIsAlerted;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "NormalMonster")
	bool bIsFirstResponder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NormalMonster")
	int32 MonsterAlertDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
	UAnimMontage* AlertMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float AlertDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bShouldRotateToPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float DeadMontageTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float DestroyTime;


	virtual void AlertNearbyMonsters_Implementation(const int32 AlertDistance) override;

	virtual void FoundPlayer_Implementation() override;

	virtual void SetAlerted_Implementation(const bool AlertValue) override;

	UFUNCTION(BlueprintCallable, Category = "Monster")
	virtual void PlayAlertMontage() override;

	void Dead() override;

	void DeadLogic();
	void DestroyMonster();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION(BlueprintCallable, Category = "Monster")
	void ResetAlert();

	UFUNCTION(BlueprintCallable, Category = "Monster")
	void BehaviorOn();

	UFUNCTION()
	void RotateToPlayer();

private:
	FTimerHandle AlertDelayTimer;
};
