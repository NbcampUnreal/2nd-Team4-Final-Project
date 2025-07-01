// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_NpcBase.h"
#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif
#include "DW_NPC_TownFolk.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API ADW_NPC_TownFolk : public ADW_NpcBase
{
	GENERATED_BODY()
	
public:
	ADW_NPC_TownFolk();
	void BeginPlay();
	// virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditInstanceOnly, Category="Patrol")
	AActor* PatrolAnchorPoint;

	UPROPERTY(EditAnywhere, Category="Patrol")
	float RoamingRadius = 3000.0f;

	UPROPERTY()
	class UNavigationInvokerComponent* NavInvokerComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="AI")
	TArray<AActor*> PatrolPoints;
};
