// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_Chest.h"

#include "Item/WorldItemActor.h"
#include "Monster/DW_MonsterBase.h"


// Sets default values
ADW_Chest::ADW_Chest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(SceneComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMeshComp->SetupAttachment(SceneComp);
}

void ADW_Chest::Interact_Implementation(AActor* Interactor)
{
	if (bHasOpened) return;

	bHasOpened = true;

	if (bIsChest)
	{
		if (IsValid(ChestMontage))
		{
			UAnimMontage* Montage = ChestMontage;
		
			if (Montage && SkeletalMeshComp && SkeletalMeshComp->GetAnimInstance())
			{
				SkeletalMeshComp->GetAnimInstance()->Montage_Play(Montage);
			}
		}
	}
	else
	{
		SpawnMimic();
	}
}

void ADW_Chest::SpawnMimic()
{
	SetActorEnableCollision(false);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<ADW_MonsterBase>(
	Mimic,
	GetActorLocation(),
	GetActorRotation(),
	SpawnParams
	);

	Destroy();
	
}

void ADW_Chest::DropItem()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AWorldItemActor* ItemActor = GetWorld()->SpawnActor<AWorldItemActor>(
		WorldItemActor,
		GetActorLocation(),
		GetActorRotation(),
		SpawnParams
	);
	
	ItemActor->SetItemCode(ItemCode);
}
