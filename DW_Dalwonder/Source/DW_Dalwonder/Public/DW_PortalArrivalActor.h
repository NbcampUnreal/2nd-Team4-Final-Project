// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_Portal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "DW_PortalArrivalActor.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_PortalArrivalActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADW_PortalArrivalActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	EPortalType PortalType = EPortalType::Portal1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UNiagaraSystem* SpawnNS = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
