// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_MonsterBaseInterface.h"
#include "MonsterTypes.h"
#include "GameFramework/Character.h"
#include "DW_MonsterBase.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_MonsterBase : public ACharacter, public IDW_MonsterBaseInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_MonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	EMonsterState CurrentState = EMonsterState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> AnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UDataTable* DataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent* AttackSoundComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent* HitSoundComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<USoundBase*> AttackSounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound;

	class DW_CharacterBase* PlayerCharacter;
	
	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	// 현재 몬스터 상태를 반환
	virtual EMonsterState GetCurrentState() const override;

	// 현재 몬스터 상태를 설정
	virtual void SetCurrentState(EMonsterState MonsterState) override;

	// 데이터 테이블을 통해 몬스터 스탯 설정
	virtual void SetStats(UDataTable* DataTable) override;

	// 몬스터 이름 반환
	virtual FName GetMonsterName() const override;

	// 몬스터 현재 HP 반환
	virtual float GetMonsterHP() const override;

	// 몬스터 공격력 반환
	virtual float GetMonsterDamage() const override;

	// 몬스터 이동 속도 반환
	virtual float GetMonsterSpeed() const override;

	// 공격 실행 (패턴 인덱스 기반)
	virtual void PerformAttack(int32 PatternIndex) override;

	// 공격용 몽타주 인덱스 랜덤 반환
	virtual int32 GetRandomMontage() override;

	// 블랙보드에 랜덤 공격 키 설정
	virtual void SetRandomAttackKey(int32 PatternIndex) override;

	// 공격 사운드 재생
	virtual void PlayAttackSound(int32 SoundIndex) override;

	// 피격 사운드 재생
	virtual void PlayHitSound() override;

	// 데미지를 받을 때 호출
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	// 데미지를 가할 때 호출
	virtual float ApplyDamage(
		AActor* DamagedActor,
		float BaseDamage,
		AController* EventInstigator,
		AActor* DamageCauser,
		TSubclassOf<UDamageType> DamageTypeClass
	) override;

	// 플레이어 캐릭터 캐스팅
	virtual void CastPlayerCharacter() override;

	// 플레이어 캐릭터 참조 반환
	virtual class DW_CharacterBase GetPlayerCharacter() override;

	// 플레이어와의 거리 반환
	virtual float GetPlayerDistance() override;
	
};
