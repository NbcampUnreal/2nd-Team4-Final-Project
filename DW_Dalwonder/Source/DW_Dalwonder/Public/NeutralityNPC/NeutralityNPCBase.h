// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCInteraction.h"
#include "NeutralityNPCBase.generated.h"

UCLASS()
class DW_DALWONDER_API ANeutralityNPCBase : public ACharacter, public INPCInteraction
{
	GENERATED_BODY()

public:
	ANeutralityNPCBase();

	virtual void OnInteract(AActor* Interactor) override;

protected:
	virtual void BeginPlay() override;

};
