// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Dissolve/DissolveComponentEyes.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UDissolveComponentEyes::UDissolveComponentEyes()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDissolveComponentEyes::BeginPlay()
{
	Super::BeginPlay();

	CurrentTime = 0.0f;
	DynamicMaterialMap.Empty();

	GetOwner()->GetComponents<UStaticMeshComponent>(StaticMeshComps);
	GetOwner()->GetComponents<USkeletalMeshComponent>(SkeletalMeshComps);

	/*for (UStaticMeshComponent* Comp : StaticMeshComps)
	{
		if (Comp)
		{
			AllMeshComponents.Add(Comp);
		}
	}

	for (USkeletalMeshComponent* Comp : SkeletalMeshComps)
	{
		if (Comp)
		{
			AllMeshComponents.Add(Comp);
		}
	}*/

	if (USceneComponent* Root = GetOwner()->GetRootComponent())
	{
		FindAllMeshComponentsRecursive(Root);  
	}

	SetComponentTickEnabled(true);
}

void UDissolveComponentEyes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentTime += DeltaTime;

	if (bIsDissolveFirst)
	{
		if (DissolveDuration <= 0.f) return;

		
			float Alpha = FMath::Clamp(CurrentTime / DissolveDuration, 0.f, 1.f);
			float Value = FMath::Lerp(StartValue, EndValue, Alpha);
			float FadeOutValue = FMath::Lerp(0.f, 0.5f, Alpha);

				for (auto& Elem : DynamicMaterialMap)
				{
					if (Elem.Value)
					{
						Elem.Value->SetScalarParameterValue(DissolveAmountParameterName, Value);
						Elem.Value->SetScalarParameterValue(FadeOutParameterName, FadeOutValue);

					}
				}

				for (UNiagaraComponent* Niagara : NiagaraComps)
				{
					if (Niagara)
					{
						Niagara->SetVariableFloat(FName("Amount"), Value);
					}
				}
			
	}
	else
	{
		
			float Alpha = FMath::Clamp(CurrentTime / 3.f, 0.f, 1.f);
			float Value = FMath::Lerp(0.f, -1.f, Alpha);
			float ValueDW = FMath::Lerp(0.1f, 0.f, Alpha);
			float ValuePW = FMath::Lerp(0.5f, 0.f, Alpha);

			for (UNiagaraComponent* Niagara : NiagaraComps)
			{
				if (Niagara)
				{
					Niagara->SetVariableFloat(FName("Amount"), Value);
					Niagara->SetVariableFloat(FName("DissolveWidth"), ValueDW);
					Niagara->SetVariableFloat(FName("ParticleWidth"), ValuePW);
				}
			}
	}
}

void UDissolveComponentEyes::DissolveStart(int32 NiagaraIndex, int32 TextureIndex, float Duration)
{
	CurrentTime = 0.0f;
	DynamicMaterialMap.Empty();
	NiagaraComps.Empty();

	

		if (bApplyRandomTexture)
		{
			TextureIndex = FMath::RandRange(0, DissolveTexture.Num() - 1);
		}

		DissolveDuration = Duration;

		for (UMeshComponent* MeshComp : AllMeshComponents)
		{
			if (!MeshComp) continue;

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
						DynMat->SetScalarParameterValue(FadeOutParameterName, StartValue);
					}

					FMeshMaterialKey Key(MeshComp, i);
					DynamicMaterialMap.Add(Key, DynMat);
					//DynamicMaterialMap.Add(i, DynMat);
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

				if (NiagaraComp)
				{
					NiagaraComp->SetVariableLinearColor(FName("DissolveColor"), DissolveParticleColor);
					NiagaraComp->SetVariableLinearColor(FName("ParticleColor"), DustParticleColor);
					NiagaraComps.Add(NiagaraComp);
				}
			}
		}

	bIsDissolveFirst = true;
	//PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(ZeroTimer, this, &UDissolveComponentEyes::DissolveZero, DissolveDuration, false);
	}

}

void UDissolveComponentEyes::DissolveZero()
{
	bIsDissolveFirst = false;
	CurrentTime = 0.0f;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &UDissolveComponentEyes::DissolveEnd, 3.f, false);
	}
}

void UDissolveComponentEyes::DissolveEnd()
{
	//PrimaryComponentTick.bCanEverTick = false;
	SetComponentTickEnabled(false);
}

void UDissolveComponentEyes::FindAllMeshComponentsRecursive(USceneComponent* Root)
{
	if (!Root) return;

	if (UMeshComponent* MeshComp = Cast<UMeshComponent>(Root))
	{
		AllMeshComponents.Add(MeshComp);
	}

	const TArray<USceneComponent*>& Children = Root->GetAttachChildren();
	for (USceneComponent* Child : Children)
	{
		FindAllMeshComponentsRecursive(Child);
	}
}

