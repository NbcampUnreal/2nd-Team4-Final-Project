// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Dissolve/DissolveComponent.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UDissolveComponent::UDissolveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDissolveComponent::BeginPlay()
{
	Super::BeginPlay();

	MeshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	CurrentTime = 0.0f;
	DynamicMaterialMap.Empty();

	SetComponentTickEnabled(true);
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

		if (NiagaraComp)
		{
			NiagaraComp->SetVariableFloat(FName("Amount"), Value);
		}
	}
	else
	{
		float Alpha = FMath::Clamp(CurrentTime / 3.f, 0.f, 1.f);
		float Value = FMath::Lerp(0.f, -1.f, Alpha);
		float ValueDW = FMath::Lerp(0.1f, 0.f, Alpha);
		float ValuePW = FMath::Lerp(0.5f, 0.f, Alpha);

		if (NiagaraComp)
		{
			NiagaraComp->SetVariableFloat(FName("Amount"), Value);
			NiagaraComp->SetVariableFloat(FName("DissolveWidth"), ValueDW);
			NiagaraComp->SetVariableFloat(FName("ParticleWidth"), ValuePW);
		}
	}
}

void UDissolveComponent::DissolveStart(int32 NiagaraIndex, int32 TextureIndex, float Duration)
{
	CurrentTime = 0.0f;

	if (!MeshComp) return;

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

			if (DynMat && DissolveTexture[TextureIndex])
			{
				DynMat->SetTextureParameterValue(TextureParameterName, DissolveTexture[TextureIndex]);
				DynMat->SetScalarParameterValue(DissolveAmountParameterName, StartValue);
				DynMat->SetScalarParameterValue(DissolveEdgeParameterName, MaterialEdgeThickness);
				DynMat->SetScalarParameterValue(EmissiveIntansityParameterName, MaterialEmissiveIntansity);
				DynMat->SetVectorParameterValue(ColorParameterName, MaterialEdgeColor);
				
			}

			DynamicMaterialMap.Add(i, DynMat);
		}
	}

	if (DissolveNiagaraSystems[NiagaraIndex])
	{
		NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			DissolveNiagaraSystems[NiagaraIndex],
			MeshComp,
			NAME_None, //FName("pelvis")
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			false
		);

		NiagaraComp->SetVariableLinearColor(FName("DissolveColor"), DissolveParticleColor);
		NiagaraComp->SetVariableLinearColor(FName("ParticleColor"), DustParticleColor);
	}

	bIsDissolveFirst = true;
	//PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(ZeroTimer, this, &UDissolveComponent::DissolveZero, DissolveDuration, false);
	}

}

void UDissolveComponent::DissolveZero()
{
	bIsDissolveFirst = false;
	CurrentTime = 0.0f;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &UDissolveComponent::DissolveEnd, 3.f, false);
	}
}

void UDissolveComponent::DissolveEnd()
{
	//PrimaryComponentTick.bCanEverTick = false;
	SetComponentTickEnabled(false);
}

