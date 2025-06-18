// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "MobEye.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API AMobEye : public ADW_NormalMonsterBase
{
	GENERATED_BODY()

protected:
	AMobEye();

	bool bIsDead = false;

public:
	virtual float TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser
) override;

	void Dead() override;

	UFUNCTION(BlueprintCallable)
	void DeadLogic() override;

	UFUNCTION(BlueprintImplementableEvent)
	void EyeDeadLogic();
};
