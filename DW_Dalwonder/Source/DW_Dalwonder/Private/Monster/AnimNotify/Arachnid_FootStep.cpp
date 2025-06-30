// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/Arachnid_FootStep.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

void UArachnid_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !NiagaraSystem) return;

	FName TargetSocket = ResolveSocketName();
	if (TargetSocket == NAME_None) return;

	FVector SpawnLocation = MeshComp->GetSocketLocation(TargetSocket) + LocationOffset;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		MeshComp->GetWorld(),
		NiagaraSystem,
		SpawnLocation,
		FRotator::ZeroRotator
	);
}

FName UArachnid_FootStep::ResolveSocketName() const
{
	switch (SocketType)
	{
	case EEffectSocketType::Front_Left:  return FName("Left");
	case EEffectSocketType::Front_Right: return FName("Right");
	case EEffectSocketType::Middle_Left:  return FName("Left2");
	case EEffectSocketType::Middle_Right: return FName("Right2");
	case EEffectSocketType::Back_Left:     return FName("Left3");
	case EEffectSocketType::Back_Right:     return FName("Right3");
	case EEffectSocketType::Face_Left:     return FName("Front2");
	case EEffectSocketType::Face_Right:     return FName("Front2");
	case EEffectSocketType::Root:				return FName("Root");
	default:                           return NAME_None;
	}
}
