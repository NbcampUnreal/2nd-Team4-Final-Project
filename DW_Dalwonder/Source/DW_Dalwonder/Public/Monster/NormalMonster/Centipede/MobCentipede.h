// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "MobCentipede.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API AMobCentipede : public ADW_NormalMonsterBase
{
	GENERATED_BODY()
	
protected:
	AMobCentipede();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void CanRotationSetting(bool Value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float InterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bCanRotation = false;
};
