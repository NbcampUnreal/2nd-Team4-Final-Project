// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "NiagaraSystem.h"
#include "ProjectileSpawnerComponent.generated.h"

/**
* 발사체를 사용하기 위해 위치를 정해줄 컴포넌트.
* 나이아가라 시스템을 배열에 추가해두면
* 몽타주에서 애님 노티파이 SpawnProjectile으로 발사됨.
* 발사체의 세부 속성은 각각의 발사체 블루프린트에서 변경 가능.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DW_DALWONDER_API UProjectileSpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UProjectileSpawnerComponent();

public:	
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnProjectile(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnConnectWithTarget(int32 Index, float DamageIncrease);

	// BP_Projectile 액터 할당 : 발사체에 대한 배열입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Projectile Spawn")
	TArray<TSubclassOf<AActor>> NiagaraSystemArray;
	// Niagara System Asset 할당 : 몬스터와 플레이어가 이어지는 이펙트 공격(번개 등)을 실행할 에셋에 대한 배열입니다.
	// 노티파이 NS_TraceSpawn을 사용해 작동시킬 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Projectile Spawn")
	TArray<UNiagaraSystem*> ConnectNiagaraSystemArray;

	// 발사시 플레이어를 향할지 여부입니다. False일 경우 컴포넌트가 바라보고있는 정면으로 발사됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Spawn")
	bool bUseLineTrace = true;
	// bUseLineTrace가 false인 경우, 발사 방향을 전방으로 할지 여부입니다. False일 경우 반대로 발사됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Niagara|Spawn")
	bool bVectorReverse = false;
};
