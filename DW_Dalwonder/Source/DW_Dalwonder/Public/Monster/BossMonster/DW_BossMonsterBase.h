// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_BossMonsterBaseInterface.h"
#include "Monster/DW_MonsterBase.h"
#include "DW_BossMonsterBase.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_BossMonsterBase : public ADW_MonsterBase, public IDW_BossMonsterBaseInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_BossMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere)
	int32 CurrentPhase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGM")
	USoundBase* BGM;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BGM")
	UAudioComponent* BGMSoundComponent;

	virtual int32 GetCurrentPhase() override;

	// C++, Blackboard의 CurrentPhase 값을 바꿈
	virtual void SetCurrentPhase(int32 NewPhase) override;
	
	virtual void SetBGM(USoundBase* NewBGM) override;	
};
