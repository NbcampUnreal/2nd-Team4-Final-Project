// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_MonsterBaseInterface.h"
#include "MonsterTypes.h"
#include "GameFramework/Character.h"
#include "Interface/BearableInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "DW_MonsterBase.generated.h"


UCLASS()
class DW_DALWONDER_API ADW_MonsterBase : public ACharacter, public IDW_MonsterBaseInterface, public IBearableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_MonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	

	EMonsterState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> AnimMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> HitMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* SpawnMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* ParriedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* DeadMontage;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	class UDataTable* DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UDataTable* DropTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent* AttackSoundComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent* HitSoundComponent;

	UPROPERTY(VIsibleAnywhere, Category = "Component")
	USceneComponent* TraceStart;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* TraceEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Base")
	TArray<USoundBase*> AttackSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Base")
	TArray<USoundBase*> HitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* HitNS;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	class ADW_CharacterBase* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EMonsterName MonsterName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MonsterMaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MonsterHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MonsterDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MonsterSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float MonsterAccelSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float MonsterDamageMultiplier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsInvincible = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bCanParried;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bIsDead;

	UPROPERTY(EditAnywhere, Category = "Attack|Debug")
	float DebugDrawTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Attack|Debug")
	bool bDrawDebugTrace = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|Debug")
	bool bUseOtherHand = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FVector PrevTraceStartVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FVector PrevTraceEndVector;
	
	UPROPERTY()
	class UNavigationInvokerComponent* NavInvokerComp;

	UFUNCTION(BlueprintCallable, Category = "Monster")
	void ResetRoot();

	UPROPERTY()
	TSet<AActor*> AlreadyAttackingActors;

	// 몬스터의 최대 속도 정의
	UFUNCTION(BlueprintCallable)
	virtual void SetMovementSpeed(int32 const NewSpeed) override;

	// 몬스터의 속도 가속 정의
	UFUNCTION(BlueprintCallable)
	virtual void SetAccelerationSpeed(int32 const NewAccelerationSpeed) override;
	
	// 현재 몬스터 상태를 반환
	virtual EMonsterState GetCurrentState() const override;

	// 현재 몬스터 상태를 설정
	virtual void SetCurrentState(EMonsterState MonsterState) override;

	// 데이터 테이블을 통해 몬스터 스탯 설정
	virtual void SetStats(UDataTable* NewDataTable) override;

	// 몬스터 이름 반환
	virtual FName GetMonsterName() const override;

	// 몬스터 현재 HP 반환
	virtual float GetMonsterHP() const override;

	// 몬스터 공격력 반환
	virtual float GetMonsterDamage() const override;

	// 몬스터 이동 속도 반환
	virtual float GetMonsterSpeed() const override;

	UFUNCTION(BlueprintCallable)
	virtual void SetMonsterDamageMultiplier(float NewMultiplier) override;

	// 몬스터 최대 속도 설정
	UFUNCTION(BlueprintCallable)
	virtual void SetMonsterSpeed(float NewSpeed) override;

	// 몬스터 가속 설정
	UFUNCTION(BlueprintCallable)
	virtual void SetMonsterAccelSpeed(float NewAccelSpeed) override;

	// 공격 실행 (패턴 인덱스 기반)
	UFUNCTION(BlueprintCallable)
	virtual void PerformAttack(int32 PatternIndex) override;

	UFUNCTION(BlueprintCallable)
	virtual void InitialSpawn() override;

	UFUNCTION(BlueprintCallable)
	virtual void PlayParryingMontage() override;

	UFUNCTION(BlueprintCallable)
	virtual void PlayHitMontage() override;
	
	// 공격용 몽타주 인덱스 랜덤 반환
	UFUNCTION(BlueprintCallable)
	virtual int32 GetRandomMontage() override;

	// 블랙보드에 랜덤 공격 키 설정
	// UFUNCTION(BlueprintCallable)
	// virtual void SetRandomAttackKey(int32 PatternIndex) override;

	// 공격 사운드 재생
	UFUNCTION(BlueprintCallable)
	virtual void PlayAttackSound(int32 SoundIndex) override;

	// 피격 사운드 재생
	UFUNCTION(BlueprintCallable)
	virtual void PlayHitSound(int32 SoundIndex) override;

	virtual void CanParry() override;

	virtual void CantParry() override;

	virtual bool GetCanParry() override;

	// 공격 판정 시작
	virtual void StartAttackTrace() override;

	// 공격 판정 종료
	virtual void EndAttackTrace() override;

	virtual void PerformAttackTrace() override;

	UFUNCTION(BlueprintCallable)
	virtual void Parried() override;

	virtual void Dead() override;



	// 데미지를 받을 때 호출
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	// 플레이어 캐릭터 캐스팅
	virtual void CastPlayerCharacter() override;

	// 플레이어 캐릭터 참조 반환
	UFUNCTION(BlueprintCallable)
	virtual class ADW_CharacterBase* GetPlayerCharacter() override;

	// 플레이어와의 거리 반환
	UFUNCTION(BlueprintCallable)
	virtual float GetPlayerDistance() override;
	virtual bool CanBeCut_Implementation(const FHitResult& Hit) override;

	virtual void DropItem(UDataTable* NewDataTable) override;
};
