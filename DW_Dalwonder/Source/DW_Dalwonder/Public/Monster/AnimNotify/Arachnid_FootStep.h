// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NiagaraSystem.h"
#include "Arachnid_FootStep.generated.h"

UENUM(BlueprintType)
enum class EEffectSocketType : uint8
{
	Root					UMETA(DisplayName = "Root"),
	Front_Left     UMETA(DisplayName = "Front_Left"),
	Front_Right    UMETA(DisplayName = "Front_Right"),
	Middle_Left     UMETA(DisplayName = "Middle_Left"),
	Middle_Right    UMETA(DisplayName = "Middle_Right"),
	Back_Left        UMETA(DisplayName = "Back_Left"),
	Back_Right       UMETA(DisplayName = "Back_Right"),
	Face_Left        UMETA(DisplayName = "Face_Left"),
	Face_Right       UMETA(DisplayName = "Face_Right")
};

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UArachnid_FootStep : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* NiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Effect")
	EEffectSocketType SocketType = EEffectSocketType::Front_Left;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FVector LocationOffset = FVector::ZeroVector;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	FName ResolveSocketName() const;
};
