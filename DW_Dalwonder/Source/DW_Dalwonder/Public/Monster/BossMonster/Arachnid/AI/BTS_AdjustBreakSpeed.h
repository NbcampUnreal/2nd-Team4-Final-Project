// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_AdjustBreakSpeed.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UBTS_AdjustBreakSpeed : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_AdjustBreakSpeed();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Speed")
	bool bIsActor = true;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float BreakTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float SlowDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float BreakSpeed = 150.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float WalkSpeed = 300.f;

	FVector TargetLocation;
};
