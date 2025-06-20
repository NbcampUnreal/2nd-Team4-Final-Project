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
	void ChangingAttackTrace(int32 Value	);

protected:

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float InterpSpeed = 5.0f;

protected:
	bool bCanRotate = true;

	bool bIsPhaseTwo = false;
	bool bCanBurrow = false;
};
