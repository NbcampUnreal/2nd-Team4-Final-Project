// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify/AnimNotify_SpawnTelegraph.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void UAnimNotify_SpawnTelegraph::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UWorld* World = MeshComp->GetWorld();
	if (!World) return;

	if (AActor* Owner = MeshComp->GetOwner())
	{
		FVector SpawnLocation = Owner->GetActorLocation();

		if(bIsTargeting)
		{
			ACharacter* Player = UGameplayStatics::GetPlayerCharacter(MeshComp->GetWorld(), 0);
			if (Player)
			{
				FVector Start = Player->GetActorLocation();
				FVector End = Start - FVector(0.f, 0.f, 1000.f);

				FHitResult HitResult;
				FCollisionQueryParams TraceParams(FName(TEXT("TelegraphTrace")), false, Player);
				TraceParams.bReturnPhysicalMaterial = false;

				if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
				{
					SpawnLocation = HitResult.Location + FVector(0.f, 0.f, 10.f);
				}
				else
				{
					SpawnLocation = Player->GetActorLocation();
				}
			}
		}
		else
		{
				FVector Start = Owner->GetActorLocation();
				FVector End = Start - FVector(0.f, 0.f, 1000.f);

				FHitResult HitResult;
				FCollisionQueryParams TraceParams(FName(TEXT("TelegraphTrace")), false, Owner);
				TraceParams.bReturnPhysicalMaterial = false;

				if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
				{
					SpawnLocation = HitResult.Location + FVector(0.f, 0.f, 10.f);
				}
				else
				{
					SpawnLocation = Owner->GetActorLocation();
				}
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		MeshComp->GetWorld()->SpawnActor<ATelegraghActor>(
			TelegraphActorClass,
			SpawnLocation,
			FRotator::ZeroRotator,
			SpawnParams
		);

	}
}
