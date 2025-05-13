// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NPCInteraction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNPCInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DW_DALWONDER_API INPCInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnInteract(AActor* Interactor) = 0;
};
