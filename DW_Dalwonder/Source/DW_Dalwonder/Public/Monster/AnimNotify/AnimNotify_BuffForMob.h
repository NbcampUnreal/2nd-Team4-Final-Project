// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BuffForMob.generated.h"

UENUM(BlueprintType)
enum class EBuffName : uint8
{
	SKN_Enhance = 0,
	SKN_EnergeShield = 1
};

/**
 * 주변 몬스터들을 타겟으로 지정해 버프 등을 제공할 수 있는 노티파이.
 * 몬스터 클래스를 불러와 해당하는 함수를 호출하는 용도로 사용.
 */
UCLASS()
class DW_DALWONDER_API UAnimNotify_BuffForMob : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Buff")
	EBuffName BuffName;

	UPROPERTY(EditAnywhere, Category = "Buff")
	int NumberOfMobAffected = 1;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
