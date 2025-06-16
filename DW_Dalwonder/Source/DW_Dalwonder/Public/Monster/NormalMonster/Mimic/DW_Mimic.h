// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_InteractItemBase.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "DW_Mimic.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Mimic : public ADW_NormalMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_Mimic();
	
	// virtual void Interact_Implementation(AActor* Interactor) override;
	//
	// virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void BeginPlay() override;

	void MimicAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* AttackNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* BeginPlayNS;
};
