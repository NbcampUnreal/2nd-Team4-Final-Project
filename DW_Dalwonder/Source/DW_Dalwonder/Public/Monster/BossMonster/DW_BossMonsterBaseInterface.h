// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DW_BossMonsterBaseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDW_BossMonsterBaseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DW_DALWONDER_API IDW_BossMonsterBaseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual int32 GetCurrentPhase() = 0;

	virtual void SetCurrentPhase(int32 NewPhase) = 0;

	// virtual void SetPhaseBlackboardValues(int32 NewPhase) = 0;

	virtual void SetBGM(USoundBase* NewBGM) = 0;

	virtual void DoDissolve(float DissolveTime) = 0;
};
