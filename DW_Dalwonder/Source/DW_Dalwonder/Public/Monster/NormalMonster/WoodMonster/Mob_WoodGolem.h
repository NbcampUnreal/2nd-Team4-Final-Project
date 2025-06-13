// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "Mob_WoodGolem.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API AMob_WoodGolem : public ADW_NormalMonsterBase
{
	GENERATED_BODY()
	
	AMob_WoodGolem();

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void UseFirstSkill();

	UFUNCTION(BlueprintCallable)
	void BackToNature();

	/*UFUNCTION(BlueprintCallable)
	void JumpOn();

	UFUNCTION(BlueprintCallable)
	void JumpOff();*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* FirstSkill;

	//bool bIsJumping = false;
};
