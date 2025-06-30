// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_MonsterBaseInterface.h"
#include "MonsterTypes.h"
#include "GameFramework/Character.h"
#include "Interface/BearableInterface.h"
#include "NiagaraComponent.h"
#include "CoreMinimal.h"
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

	// 몬스터가 죽을 때 스폰한 이펙트 컴포넌트
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> SpawnedVFX;
	
	virtual void Tick(float DeltaTime) override;

public:
	

	// 현재 몬스터의 State입니다. (더미)
	EMonsterState CurrentState;

	// 공격 몽타주 배열입니다. (필수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> AnimMontages;

	// 피격 몽타주 배열입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> HitMontages;

	// 첫 스폰 시의 몽타주입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* SpawnMontage;

	// 패링 당했을 때의 몽타주입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* ParriedMontage;

	// 사망 시의 몽타주입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* DeadMontage;

	// 몬스터의 스탯 테이블입니다. (필수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	class UDataTable* DataTable;

	// 몬스터의 드랍 테이블입니다. (필수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UDataTable* DropTable;

	// 공격 시의 사운드 컴포넌트입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent* AttackSoundComponent;

	// 피격 시의 사운드 컴포넌트입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent* HitSoundComponent;

	// 트레이스의 시작점입니다.
	UPROPERTY(VIsibleAnywhere, Category = "Component")
	USceneComponent* TraceStart;

	// 트레이스의 끝점입니다.
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* TraceEnd;

	// 공격 시의 사운드 배열입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Base")
	TArray<USoundBase*> AttackSounds;

	// 피격 시의 사운드 배열입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Base")
	TArray<USoundBase*> HitSounds;

	// 피격 시의 나이아가라 이펙트입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* HitNS;

	// 방어 시의 나이아가라 이펙트입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* GuardHitNS;

	// 캐릭터 피격 시의 나이아가라 이펙트입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* HitImpactNS;

	// 플레이어 캐릭터의 참조입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	class ADW_CharacterBase* PlayerCharacter;

	// 몬스터의 이름입니다. MonsterTypes.h에서 선언 후 지정할 수 있습니다. (필수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EMonsterName MonsterName;

	// 몬스터의 최대체력입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MonsterMaxHP;

	// 몬스터의 현재체력입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MonsterHP;

	// 몬스터의 공격력입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MonsterDamage;

	// 몬스터의 최대 속력입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float MonsterSpeed;

	// 몬스터의 가속력입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float MonsterAccelSpeed;

	// 몬스터 공격 데미지의 배율입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float MonsterDamageMultiplier;

	// 몬스터의 무적 판정입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsInvincible = false;

	// 몬스터의 가드 상태입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bIsGuard = false;

	// 트레이스 공격이 넉백을 유발합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bCanKnockbackByAttack = false;

	// 몬스터의 공격 상태입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bIsAttacking;

	// 몬스터의 패리 가능 여부 상태입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bCanParried;

	// 몬스터의 사망 여부입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bIsDead;

	// 트레이스 시 디버그 라인의 시간입니다.
	UPROPERTY(EditAnywhere, Category = "Attack|Debug")
	float DebugDrawTime = 1.0f;

	// 트레이스 시 디버그 라인의 여부입니다.
	UPROPERTY(EditAnywhere, Category = "Attack|Debug")
	bool bDrawDebugTrace = true;

	// 모르겠읍니다
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|Debug")
	bool bUseOtherHand = false;

	// 트레이스 시 사용되는 변수입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FVector PrevTraceStartVector;
	// 트레이스 시 사용되는 변수입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FVector PrevTraceEndVector;

	UPROPERTY(EditDefaultsOnly, Category = "HitReaction")
	TSubclassOf<UCameraShakeBase> DefaultHitCameraShake;

	FTimerHandle HitStopTimerHandle;

	// NavInvoker 컴포넌트입니다.
	UPROPERTY()
	class UNavigationInvokerComponent* NavInvokerComp;

	// 모르겠읍니다
	UFUNCTION(BlueprintCallable, Category = "Monster")
	void ResetRoot();

	// 트레이스 시 중복 액터를 제외합니다.
	UPROPERTY()
	TSet<AActor*> AlreadyAttackingActors;

	// 몬스터의 최대 속력를 정의합니다.
	UFUNCTION(BlueprintCallable)
	virtual void SetMovementSpeed(int32 const NewSpeed) override;

	// 몬스터의 가속력을 정의합니다.
	UFUNCTION(BlueprintCallable)
	virtual void SetAccelerationSpeed(int32 const NewAccelerationSpeed) override;
	
	// 현재 몬스터 상태를 반환합니다. (더미)
	virtual EMonsterState GetCurrentState() const override;

	// 현재 몬스터 상태를 설정합니다. (더미)
	virtual void SetCurrentState(EMonsterState MonsterState) override;

	// 데이터 테이블을 통해 몬스터 스탯을 설정합니다.
	virtual void SetStats(UDataTable* NewDataTable) override;

	// 데이터 테이블을 기반으로 숙련도를 획득합니다.
	virtual void IncreaseMastery(UDataTable* NewDataTable) override;

	// 몬스터의 이름을 반환합니다.
	virtual FName GetMonsterName() const override;

	// 몬스터의 최대 HP를 반환합니다.
	virtual float GetMonsterMaxHP() const override;

	// 몬스터 현재 HP를 반환합니다.
	virtual float GetMonsterHP() const override;

	// 몬스터의 공격력을 반환합니다.
	virtual float GetMonsterDamage() const override;

	// 몬스터의 최대 속력을 반환합니다.
	virtual float GetMonsterSpeed() const override;

	// 몬스터의 데미지 배율을 정의합니다.
	UFUNCTION(BlueprintCallable)
	virtual void SetMonsterDamageMultiplier(float NewMultiplier) override;

	// 몬스터의 최대 속력을 정의합니다.
	UFUNCTION(BlueprintCallable)
	virtual void SetMonsterSpeed(float NewSpeed) override;

	// 몬스터의 가속력을 정의합니다.
	UFUNCTION(BlueprintCallable)
	virtual void SetMonsterAccelSpeed(float NewAccelSpeed) override;

	// 공격을 실행합니다. 매개변수를 통해 AnimMontage 배열 내부 몽타주를 실행합니다.
	UFUNCTION(BlueprintCallable)
	virtual void PerformAttack(int32 PatternIndex) override;

	// 스폰 시의 몽타주를 실행합니다.
	UFUNCTION(BlueprintCallable)
	virtual void InitialSpawn() override;

	// 패링 몽타주를 실행합니다.
	UFUNCTION(BlueprintCallable)
	virtual void PlayParryingMontage() override;

	// 피격 몽타주를 실행합니다.
	UFUNCTION(BlueprintCallable)
	virtual void PlayHitMontage() override;
	
	// AnimMontages 배열 중 하나의 인덱스를 반환합니다.
	UFUNCTION(BlueprintCallable)
	virtual int32 GetRandomMontage() override;

	// 블랙보드에 랜덤 공격 키 설정
	// UFUNCTION(BlueprintCallable)
	// virtual void SetRandomAttackKey(int32 PatternIndex) override;

	// 공격 사운드를 재생합니다.
	UFUNCTION(BlueprintCallable)
	virtual void PlayAttackSound(const int32 SoundIndex) override;

	// 피격 사운드를 재생합니다.
	UFUNCTION(BlueprintCallable)
	virtual void PlayHitSound() override;

	// 패링 가능 여부를 설정합니다.
	virtual void CanParry() override;

	// 패링 가능 여부를 설정합니다.
	virtual void CantParry() override;

	// 패링 가능 여부를 반환합니다.
	virtual bool GetCanParry() override;

	// 트레이스를 시작합니다.
	virtual void StartAttackTrace() override;

	// 트레이스를 종료합니다.
	virtual void EndAttackTrace() override;

	// 실질적인 트레이스 함수입니다.
	virtual void PerformAttackTrace() override;

	// 패링 함수입니다.
	UFUNCTION(BlueprintCallable)
	virtual void Parried() override;

	// 사망 함수입니다.
	virtual void Dead() override;

	// 스폰된 드롭 이펙트를 파괴합니다.
	void DestroySpawnedVFX();

	// 피격 시 호출됩니다.
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	void RemoveDropVFX();

	// 플레이어 캐릭터를 캐스팅합니다.
	virtual void CastPlayerCharacter() override;

	// 플레이어 캐릭터 참조를 반환합니다.
	UFUNCTION(BlueprintCallable)
	virtual class ADW_CharacterBase* GetPlayerCharacter() override;

	// 플레이어와의 거리를 반환합니다.
	UFUNCTION(BlueprintCallable)
	virtual float GetPlayerDistance() override;

	// 캐릭터의 트레이스 시 정상적으로 벨 수 있는지의 여부를 정의합니다.
	virtual bool CanBeCut_Implementation(const FHitResult& Hit) override;

	// 아이템을 드랍합니다.
	virtual void DropItem(UDataTable* NewDataTable) override;

	// 트레이스 시 중복 액터 배열을 초기화합니다.
	virtual void ResetAttakingActors() override;

	virtual void HitStop(float StopTime) override;
};
