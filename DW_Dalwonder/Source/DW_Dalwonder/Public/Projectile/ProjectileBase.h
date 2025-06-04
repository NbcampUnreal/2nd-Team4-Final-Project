// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h" 
#include "ProjectileBase.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;

UCLASS()
class DW_DALWONDER_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

	void OnConstruction(const FTransform& Transform);


	// 피격 지점에 이펙트 생성. 생성된 이펙트는 자동 제거됨.
	void HitEffectSpawnLogic(const FHitResult& Hit);
	// Destroy를 Timer로 호출하기 위한 함수.
	void DestroyToDelay();

	// ---------- 컴포넌트 선언 ----------
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;
	// ---------- ---------- ----------

	// ---------- 변수 선언 ----------
public:
	// 발사체 충돌 가능 범위를 조절합니다. 위치는 Niagara Component의 Location을 변경해주세요.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Class")
	float CollisionRadius;
	// 발사체가 액터와 충돌 후 남아있게 할지에 대한 변수입니다. False면 사라지고 True면 유지합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Class")
	bool bRemainNiagara;
	// 발사체가 액터와 충돌했을 때부터 파괴되기까지의 시간입니다. bRemainNiagara가 True일 경우 유효합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Class")
	float HitDestroyDelay;
	// 발사체가 스폰됐을 때부터 파괴되기까지의 시간입니다. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Class")
	float DestroyDelay;

	// ProjectileMovement의 발사 속도를 조절합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Movement")
	float Speed;
	// ProjectileMovement의 중력을 조절합니다. 0일 경우 지면과 평행하게 날아갑니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Movement")
	float Gravity;
	// ProjectileMovement의 RotationFollowsVelocity 활성화 여부입니다. Gravity가 0보다 크고 해당 변수가 True면 포물선을 그리며 날아갑니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Movement")
	bool bRotationFromVelocity;
	// Hit시 캐릭터 Launch의 여부입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Launch")
	bool bDoLaunch;
	// Launch의 힘을 조절합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Launch", meta = (EditCondition = "bDoLaunch", EditConditionHides))
	float LaunchImpulse;
	// Launch의 방향을 조절합니다. True일 때, Z축으로만 날아갑니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Launch", meta = (EditCondition = "bDoLaunch", EditConditionHides))
	bool bIsZOnly;

	// 발사체가 플레이어와 충돌했을 때 스폰시킬 나이아가라 시스템 클래스입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|HitEffect")
	UNiagaraSystem* HitEffect;
	// HitEffect 스폰시 발사체가 충돌된 위치로부터 얼만큼 떨어뜨릴지에 대한 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|HitEffect")
	float HitEffectOffset;
	// HitEffect의 크기를 조절합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|HitEffect")
	float HitEffectSize;
	// HitEffect가 플레이어 외 충돌시에도 생성하게 할지에 대한 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|HitEffect")
	bool bSpawnAtLand;

	// 플레이어에게 충돌시 가할 데미지입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Damage")
	float DamageAmount;

protected:
	FTimerHandle DestroyTimer;
	// ---------- ---------- ----------
};
