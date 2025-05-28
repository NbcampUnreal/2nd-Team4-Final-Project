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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Projectile Spawn")
	TArray<TSubclassOf<AActor>> NiagaraSystemArray;
};
