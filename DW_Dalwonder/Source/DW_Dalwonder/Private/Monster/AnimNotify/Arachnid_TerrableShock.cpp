// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/Arachnid_TerrableShock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Monster/BossMonster/Arachnid/BossArachnid.h"

void UArachnid_TerrableShock::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner() && DecalMaterial)
	{
		AActor* Owner = MeshComp->GetOwner();
		ABossArachnid* Monster = Cast<ABossArachnid>(Owner);

		Monster->SpawnDecal(DecalMaterial);

		/*FVector OwnerLocation = Owner->GetActorLocation();

		FVector SpawnLocation = OwnerLocation - FVector(0.f, 0.f, Owner->GetSimpleCollisionHalfHeight());

		FRotator DecalRotation = FRotator(0.f, -90.f, 0.f);

		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
			Owner->GetWorld(),
			DecalMaterial,
			FVector(100.f, 10.f, 10.f),
			SpawnLocation,
			DecalRotation,
			0.f
		);

		if (Decal)
		{
			Decal->SetFadeOut(0.5f, 3.0f, false);
		}*/


	}

}
