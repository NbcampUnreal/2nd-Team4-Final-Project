// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/DW_MonsterBase.h"
#include "BossArachnid.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API ABossArachnid : public ADW_MonsterBase
{
	GENERATED_BODY()
	
protected:
	ABossArachnid();

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION(BlueprintCallable)
	void RotationEnable(bool Value);

	UFUNCTION(BlueprintCallable)
	void DoTrunInPlace(bool Value);

	UFUNCTION(BlueprintCallable)
	void ChangingAttackTrace(int32 Value	);

	UFUNCTION(BlueprintCallable)
	void ArachnidJumpOn();

	UFUNCTION(BlueprintCallable)
	void ArachnidJumpOff();

protected:

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arachnid")
	float InterpSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arachnid")
	float JumpXMultiplier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arachnid")
	float JumpZMultiplier = 1.f;

protected:
	bool bCanRotate = true;
	bool bShouldTurn = false;
	bool bIsJumping = false;

	bool bIsPhaseTwo = false;
	bool bCanBurrow = false;
	
	FVector CurrentVector;
	float CurrentYaw;
	
};
