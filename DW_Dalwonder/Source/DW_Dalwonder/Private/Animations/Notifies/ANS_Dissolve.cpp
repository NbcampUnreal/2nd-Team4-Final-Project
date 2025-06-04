// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notifies/ANS_Dissolve.h"

void UANS_Dissolve::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp) return;

	Duration = TotalDuration;
	CurrentTime = 0.0f;
	DynamicMaterialMap.Empty();

	int32 MaterialCount = MeshComp->GetNumMaterials();

	for (int32 i = 0; i < MaterialCount; ++i)
	{
		UMaterialInterface* Mat = MeshComp->GetMaterial(i);
		if (Mat)
		{
			UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Mat, MeshComp);
			MeshComp->SetMaterial(i, DynMat);

			if (DissolveTexture)
			{
				DynMat->SetTextureParameterValue(TextureParameterName, DissolveTexture);
				DynMat->SetScalarParameterValue(DissolveAmountParameterName, StartValue);
				DynMat->SetScalarParameterValue(DissolveEdgeParameterName, EdgeThickness);
				DynMat->SetVectorParameterValue(ColorParameterName, EdgeColor);

			}

			DynamicMaterialMap.Add(i, DynMat);
		}
	}
}

void UANS_Dissolve::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!MeshComp || Duration <= 0.f) return;

	CurrentTime += FrameDeltaTime;
	float Alpha = FMath::Clamp(CurrentTime / Duration, 0.f, 1.f);
	float Value = FMath::Lerp(StartValue, EndValue, Alpha);

	for (auto& Elem : DynamicMaterialMap)
	{
		if (Elem.Value)
		{
			Elem.Value->SetScalarParameterValue(DissolveAmountParameterName, Value);
		}
	}
}

void UANS_Dissolve::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	for (auto& Elem : DynamicMaterialMap)
	{
		if (Elem.Value)
		{
			Elem.Value->SetScalarParameterValue(DissolveAmountParameterName, EndValue);
		}
	}
	DynamicMaterialMap.Empty();
}
