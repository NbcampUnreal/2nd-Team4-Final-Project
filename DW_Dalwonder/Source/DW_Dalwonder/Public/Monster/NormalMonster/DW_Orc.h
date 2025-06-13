// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_NormalMonsterBase.h"
#include "DW_Orc.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Orc : public ADW_NormalMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_Orc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
