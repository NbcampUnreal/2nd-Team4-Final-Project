// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "StarEffectActor.generated.h"

UCLASS()
class DW_DALWONDER_API AStarEffectActor : public AActor
{
	GENERATED_BODY()
	
public:
    AStarEffectActor();

    UFUNCTION()
    void DestroyEffectActor();

    UPROPERTY(VisibleAnywhere)
    UNiagaraComponent* NiagaraComp;

    void InitEffect(UNiagaraSystem* NiagaraSystem, FVector Location, FRotator Rotation);

protected:
    virtual void BeginPlay() override;

    FTimerHandle DestroyTimerHandle;
};