﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* AirAttackNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* SpawnMonsterNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* SurroundedAttackNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* RepDeadNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* BoxAttackNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsRealBoss = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	UNiagaraComponent* TrailNS;

	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Tick(float DeltaTime) override;

	void AirAttack();

	UFUNCTION(BlueprintCallable)
	void DoTeleport() const;

	UFUNCTION(BlueprintCallable)
	void DoRangedTeleport() const;

	void SpawnMonster(const TSubclassOf<ADW_MonsterBase>& SpawnMob) const;

	void SurroundedAttack();

	void BoxAttack();

	virtual void Dead() override;
};
