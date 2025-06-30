// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/Arachnid_SoundPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

void UArachnid_SoundPlayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !SoundToPlay) return;

	UWorld* World = MeshComp->GetWorld();
	if (!World) return;

	FVector Location = MeshComp->GetComponentLocation();

	UGameplayStatics::PlaySoundAtLocation(World, SoundToPlay, Location, VolumeMultiplier, PitchMultiplier);
}
