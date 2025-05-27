// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BossMonster/DW_BossMonsterBase.h"
#include "DW_Sevarog.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Sevarog : public ADW_BossMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_Sevarog();

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* Hammer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* TeleportMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* RangedTeleportMontage;

	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void AirAttack();

	UFUNCTION(BlueprintCallable)
	void DoTeleport();

	UFUNCTION(BlueprintCallable)
	void DoRangedTeleport();
};
