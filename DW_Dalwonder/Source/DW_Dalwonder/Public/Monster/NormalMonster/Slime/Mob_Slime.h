// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "Mob_Slime.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API AMob_Slime : public ADW_NormalMonsterBase
{
	GENERATED_BODY()
	
protected:
	AMob_Slime();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Landed(const FHitResult& Hit) override;

public:
	UFUNCTION(BlueprintCallable)
	void LaunchActor(float Strength, float UpwardRatio, bool bIsForward, FVector TargetLocation);

	UFUNCTION(BlueprintCallable)
	void Attack_Explose();

public:
	// 스폰시킬 TelegraphActor 블루프린트 클래스입니다.
	UPROPERTY(EditAnywhere, Category = "Telegraph")
	TSubclassOf<AActor> TelegraphActorClass;

private:
	class USkeletalMeshComponent* Mesh;

	/*FVector MeshZOffset;

	FVector InitialScale = FVector(2.f, 2.f, 2.f);
	FVector TargetScale;
	FVector StartScale;

	float ScaleDuration = 0.1f;
	float ScaleElapsed = 0.f;
	bool bIsScaling = false;

	bool bIsLandingBounce = false;
	bool bShouldBounceMesh = false;*/
};
