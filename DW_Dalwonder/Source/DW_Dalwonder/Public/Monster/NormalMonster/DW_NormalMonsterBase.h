// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_NormalMonsterBaseInterface.h"
#include "Monster/DW_MonsterBase.h"
#include "DW_NormalMonsterBase.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_NormalMonsterBase : public ADW_MonsterBase, public IDW_NormalMonsterBaseInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_NormalMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="NormalMonster")
	bool bIsAlerted;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "NormalMonster")
	bool bIsFirstResponder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NormalMonster")
	int32 MonsterAlertDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
	UAnimMontage* AlertMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float AlertDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bShouldRotateToPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float DeadMontageTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float DestroyTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDissolveComponent* DissolveComp;

	// SpawnMontage 중에 AlertMontage 재생을 막기 위한 재정의
	virtual void InitialSpawn() override;
	// SpawnMontage 종료 델리게이트 함수
	UFUNCTION()
	void OnSpawnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual void AlertNearbyMonsters_Implementation(const int32 AlertDistance) override;

	virtual void FoundPlayer_Implementation() override;

	virtual void SetAlerted_Implementation(const bool AlertValue) override;

	UFUNCTION(BlueprintCallable, Category = "Monster")
	virtual void PlayAlertMontage() override;

	void Dead() override;

	virtual void DeadLogic();
	void DestroyDissolve();
	void DestroyMonster();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION(BlueprintCallable, Category = "Monster")
	void ResetAlert();

	UFUNCTION(BlueprintCallable, Category = "Monster")
	void BehaviorOn();

	UFUNCTION()
	void RotateToPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float HitDelay;

private:
	FTimerHandle AlertDelayTimer;
	FTimerHandle HitDelayTimer;

	// SpawnMontage가 재생중인지 확인하는 변수
	bool bIsPlayingSpawnMontage = false;
	// SpawnMontage 재생중에 플레이어가 감지됐다면 AlertMontage를 즉시 실행하지 않고 예약할 변수
	bool bWantsToPlayAlertMontage = false;
};
