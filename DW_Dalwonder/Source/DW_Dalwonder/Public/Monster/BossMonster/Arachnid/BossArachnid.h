// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/DW_MonsterBase.h"
#include "BossArachnid.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API ABossArachnid : public ADW_MonsterBase
{
	GENERATED_BODY()
	
protected:
	ABossArachnid();

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float InterpSpeed = 5.0f;
};
