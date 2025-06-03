// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_BuffForMob.h"
#include "Monster/NormalMonster/Skeleton/MobSkeleton.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_BuffForMob::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	UWorld* World = MeshComp->GetWorld();
	if (!World) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	const FVector Center = Owner->GetActorLocation();
	const float SearchRadius = 2000.f;

	/*TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	bool bHit = World->OverlapMultiByChannel(
		Overlaps,
		Center,
		FQuat::Identity,
		ECC_Pawn,
		Sphere,
		QueryParams
	);*/

	TArray<AActor*> OutActors;

	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Center,
		SearchRadius,
		{ UEngineTypes::ConvertToObjectType(ECC_Pawn) },
		nullptr,
		{ Owner },
		OutActors
	);


	if (bHit)
	{
		int32 AffectedCount = 0;

		for (AActor* OverlappedActor : OutActors)
		{
			if (AffectedCount >= NumberOfMobAffected) break;
				
			if (AMobSkeleton* Mob = Cast<AMobSkeleton>(OverlappedActor))
			{
				switch (BuffName)
				{
				case EBuffName::SKN_Enhance:
					Mob->AffectToEnhance();
					break;

				case EBuffName::SKN_EnergeShield:
					Mob->AffectToEnergeShield();
					break;

				default:
					break;
				}

				++AffectedCount;
			}
		}
	}
}
