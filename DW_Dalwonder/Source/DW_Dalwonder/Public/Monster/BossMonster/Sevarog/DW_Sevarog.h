// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Monster/BossMonster/DW_BossMonsterBase.h"
#include "DW_Sevarog.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDead);

UCLASS()
class DW_DALWONDER_API ADW_Sevarog : public ADW_BossMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_Sevarog();

	UPROPERTY(BlueprintAssignable, Category="Boss")
	FOnBossDead OnBossDead;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* Hammer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* TeleportMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* RangedTeleportMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* InitPhase2Montage;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraComponent* TrailNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraComponent* Phase2TrailNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* DeadNS;
	
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

	void SetInvincible(const bool NewState);

	void DoPhase2();
	
	virtual void SetCurrentPhase(int32 NewPhase) override;
	
	virtual void Dead() override;

	void ActivateRagdoll();

	void DestroySelf();
};
