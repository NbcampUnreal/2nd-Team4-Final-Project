// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DW_NormalMonsterBaseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDW_NormalMonsterBaseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DW_DALWONDER_API IDW_NormalMonsterBaseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void AlertNearbyMonsters(int32 AlertDistance) = 0;

	virtual void FoundPlayer() = 0;

	virtual void SetAlerted(bool AlertValue) = 0;
};
