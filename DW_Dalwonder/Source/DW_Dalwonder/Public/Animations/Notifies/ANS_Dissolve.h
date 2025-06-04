// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_Dissolve.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UANS_Dissolve : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material|Texture")
	UTexture* DissolveTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material|Edge")
	FLinearColor EdgeColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material|Edge")
	float EdgeThickness = 0.0f;

private:
	FName DissolveAmountParameterName = "Amount";
	FName TextureParameterName = "DissolveTexture";
	FName ColorParameterName = "EdgeColor";
	FName DissolveEdgeParameterName = "EdgeThickness";

	TMap<int32, UMaterialInstanceDynamic*> DynamicMaterialMap;
	float CurrentTime = 0.0f;
	float Duration = 1.0f;

	float StartValue = 0.0f;
	float EndValue = 1.0f;
};
