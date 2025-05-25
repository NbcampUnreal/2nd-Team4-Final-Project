// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "DW_GeometryCollectionActor.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_GeometryCollectionActor : public AGeometryCollectionActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADW_GeometryCollectionActor();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void DestructActor();

	UFUNCTION(BlueprintCallable)
	void DestroyActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 VectorX = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 VectorY = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 VectorZ = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 ImpulseStrength = 100;

	// FTimerHandle DestroyTimerHandle;
};
