// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TelegraghActor.generated.h"

UCLASS()
class DW_DALWONDER_API ATelegraghActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATelegraghActor();

public:
	UFUNCTION()
	void OnEffectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

	void OnConstruction(const FTransform& Transform);

	// 경고 지점에 찐 이펙트 생성하기 위한 함수.
	void RealEffectSpawnLogic();
	// Destroy를 Timer로 호출하기 위한 함수.
	void DestroyToDelay();

	// ---------- 컴포넌트 선언 ----------
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComponent;

	// 사전 경고를 표기할 텔레그래프 나이아가라 시스템 에셋입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* EffectComponent;
	// ---------- ---------- ----------

	// ---------- 변수 선언 ----------
public:
	// 사전 경고인 텔레그래프를 사용할지에 대한 변수입니다. False면 TelegraphEffect가 무효되며 즉발기로 바뀝니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Class")
	bool bUseTelegraph = true;

	// 사전 경고를 표기할 텔레그래프 나이아가라 시스템 에셋입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Class")
	class UNiagaraSystem* TelegraphEffect;

	// 경고가 끝난 뒤 재생될 나이아가라 시스템 에셋입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Class")
	class UNiagaraSystem* RealEffect;

	// RealEffect의 재생을 늦추는 Delay 시간입니다. 텔레그래프 시작 시점과 동시에 계산합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Class")
	float TelegraphTime = 2.0f;

	// 텔레그래프의 LinearColor를 설정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	FLinearColor TelegraphColor = FLinearColor(1.f, 0.f, 0.f, 1.f);

	// RealEffect의 재생을 늦추는 Delay 시간입니다. 텔레그래프 시작 시점과 동시에 계산합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Class")
	float TelegraphScale = 500.f;

	// 발사체 충돌 가능 범위를 조절합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Class")
	float CollisionRadius = 500.f;

	// 플레이어에게 충돌시 가할 데미지입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara|Effect")
	float DamageAmount;
	// ---------- ---------- ----------

private:
	FTimerHandle TelegraphTimerHandle;
};
