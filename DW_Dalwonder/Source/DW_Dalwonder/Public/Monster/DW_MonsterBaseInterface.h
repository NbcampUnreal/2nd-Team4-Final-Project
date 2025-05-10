#pragma once

#include "CoreMinimal.h"
#include "MonsterTypes.h"
#include "UObject/Interface.h"
#include "DW_MonsterBaseInterface.generated.h"

UINTERFACE()
class UDW_MonsterBaseInterface : public UInterface
{
	GENERATED_BODY()
};

class DW_DALWONDER_API IDW_MonsterBaseInterface
{
	GENERATED_BODY()

public:

	// 최대 속도 정의
	virtual void SetMovementSpeed(int32 const NewSpeed) = 0;

	// 최대 가속 정의
	virtual void SetAccelerationSpeed(int32 const NewAccelerationSpeed) = 0;

	// 현재 몬스터 상태를 반환
	virtual EMonsterState GetCurrentState() const = 0;

	// 현재 몬스터 상태를 설정
	virtual void SetCurrentState(EMonsterState MonsterState) = 0;

	// 데이터 테이블을 통해 몬스터 스탯 설정
	virtual void SetStats(UDataTable* DataTable) = 0;

	// 몬스터 이름 반환
	virtual FName GetMonsterName() const = 0;

	// 몬스터 현재 HP 반환
	virtual float GetMonsterHP() const = 0;

	// 몬스터 공격력 반환
	virtual float GetMonsterDamage() const = 0;

	// 몬스터 이동 속도 반환
	virtual float GetMonsterSpeed() const = 0;

	// 공격 실행 (패턴 인덱스 기반)
	virtual void PerformAttack(int32 PatternIndex) = 0;

	virtual void InitialSpawn() = 0;

	virtual void PlayParryingMontage() = 0;

	// 공격용 몽타주 인덱스 랜덤 반환
	virtual int32 GetRandomMontage() = 0;

	// 블랙보드에 랜덤 공격 키 설정
	// virtual void SetRandomAttackKey(int32 PatternIndex) = 0;

	// 공격 사운드 재생
	virtual void PlayAttackSound(int32 SoundIndex) = 0;

	// 피격 사운드 재생
	virtual void PlayHitSound(int32 SoundIndex) = 0;

	// 패링 판정 시작(AnimNotify에서 실행 되어야 함)
	virtual void CanParry() = 0;

	// 패링 판정 종료(AnimNotify에서 실행 되어야 함)
	virtual void CantParry() = 0;

	virtual bool GetCanParry() = 0;

	// 공격 판정 시작(AnimNotify에서 실행 되어야 함)
	virtual void StartAttackTrace() = 0;

	// 공격 판정 종료(AnimNotify에서 실행 되어야 함)
	virtual void EndAttackTrace() = 0;

	virtual void PerformAttackTrace() = 0;

	virtual void Parried() = 0;

	virtual void Dead() = 0;

	// 데미지를 받을 때 호출
	// virtual float TakeDamage(
	// 	float DamageAmount,
	// 	struct FDamageEvent const& DamageEvent,
	// 	class AController* EventInstigator,
	// 	AActor* DamageCauser
	// );

	// 데미지를 가할 때 호출
	// virtual float ApplyDamage(
	// 	AActor* DamagedActor,
	// 	float BaseDamage,
	// 	AController* EventInstigator,
	// 	AActor* DamageCauser,
	// 	TSubclassOf<UDamageType> DamageTypeClass
	// );

	// 플레이어 캐릭터 캐스팅
	virtual void CastPlayerCharacter() = 0;

	// 플레이어 캐릭터 참조 반환
	virtual class ADW_CharacterBase* GetPlayerCharacter() = 0;

	// 플레이어와의 거리 반환
	virtual float GetPlayerDistance() = 0;
};
