// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Dissolve/DissolveComponent.h"
#include "TimerManager.h"

UDissolveComponent::UDissolveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UDissolveComponent::BeginPlay()
{
	Super::BeginPlay();

	MeshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	CurrentTime = 0.0f;
	DynamicMaterialMap.Empty();

	
}

void UDissolveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentTime += DeltaTime;

	if (bIsDissolveFirst)
	{
		if (!MeshComp || DissolveDuration <= 0.f) return;

		float Alpha = FMath::Clamp(CurrentTime / DissolveDuration, 0.f, 1.f);
		float Value = FMath::Lerp(StartValue, EndValue, Alpha);

		for (auto& Elem : DynamicMaterialMap)
		{
			if (Elem.Value)
			{
				Elem.Value->SetScalarParameterValue(DissolveAmountParameterName, Value);
			}
		}
	}
	else
	{
		float Alpha = FMath::Clamp(CurrentTime / DissolveDuration, 0.f, 1.f);
		float Value = FMath::Lerp(0.f, -1.f, Alpha);

		for (auto& Elem : DynamicMaterialMap)
		{
			if (Elem.Value)
			{
				Elem.Value->SetScalarParameterValue(DissolveAmountParameterName, Value);
			}
		}
	}
}

void UDissolveComponent::DissolveStart(int32 TextureIndex, float Duration)
{
	PrimaryComponentTick.bCanEverTick = true;

	if (!MeshComp) return;

	bIsDissolveFirst = true;

	if (bApplyRandomTexture)
	{
		TextureIndex = FMath::RandRange(0, DissolveTexture.Num());
	}

	DissolveDuration = Duration;

	int32 MaterialCount = MeshComp->GetNumMaterials();

	for (int32 i = 0; i < MaterialCount; ++i)
	{
		UMaterialInterface* Mat = MeshComp->GetMaterial(i);
		if (Mat)
		{
			UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Mat, MeshComp);
			MeshComp->SetMaterial(i, DynMat);

			if (DissolveTexture[TextureIndex])
			{
				DynMat->SetTextureParameterValue(TextureParameterName, DissolveTexture[TextureIndex]);
				DynMat->SetScalarParameterValue(DissolveAmountParameterName, StartValue);
				DynMat->SetScalarParameterValue(DissolveEdgeParameterName, MaterialEdgeThickness);
				DynMat->SetVectorParameterValue(ColorParameterName, MaterialEdgeColor);

			}

			DynamicMaterialMap.Add(i, DynMat);
		}
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(ZeroTimer, this, &UDissolveComponent::DissolveZero, DissolveDuration, false);
	}
}

void UDissolveComponent::DissolveZero()
{
	bIsDissolveFirst = false;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &UDissolveComponent::DissolveEnd, 1.f, false);
	}
}

void UDissolveComponent::DissolveEnd()
{
	PrimaryComponentTick.bCanEverTick = false;
}

