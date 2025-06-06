// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_CameraShake.h"

void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	const UWorld* World = MeshComp->GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC || !ShakeClass) return;

	PC->ClientStartCameraShake(ShakeClass, Scale);
}
