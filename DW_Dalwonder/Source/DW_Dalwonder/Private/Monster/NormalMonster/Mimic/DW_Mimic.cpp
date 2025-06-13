// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Mimic/DW_Mimic.h"

#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DW_CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADW_Mimic::ADW_Mimic()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ADW_Mimic::BeginPlay()
{
	Super::BeginPlay();

	if (AAIController* Ctr = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
		{
			BBC->SetValueAsObject(FName("TargetActor"), PlayerCharacter);
		}
	}

	if (!BeginPlayNS) return;
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	GetWorld(),
	BeginPlayNS,
	GetActorLocation(),
	GetActorRotation(),
	FVector(10.f),
	true,
	true
);
	
}

void ADW_Mimic::MimicAttack()
{
	const FVector Forward = GetActorForwardVector();
	const FVector AttackLocation = GetActorLocation() + Forward * 150.f + FVector(0.f, 0.f, -50.f);
	constexpr float Radius = 100.0f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		AttackLocation,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);

	if (bHit)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();
			if (HitActor && HitActor->ActorHasTag("Player"))
			{
				ADW_CharacterBase* HitCharacter = Cast<ADW_CharacterBase>(HitActor);
				HitCharacter->KnockBackCharacter();
				UGameplayStatics::ApplyDamage(HitActor, MonsterDamage * MonsterDamageMultiplier, GetController(), this, UDamageType::StaticClass());
			}
		}
	}

	if (bDrawDebugTrace)
	{
		DrawDebugSphere(GetWorld(), AttackLocation, Radius, 16, FColor::Red, false, 1.0f);


		if (!AttackNS) return;

		const FVector SpawnLocation = AttackLocation;
		const FRotator SpawnRotation = GetActorRotation();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			AttackNS,
			SpawnLocation,
			SpawnRotation,
			FVector(10.f),
			true,
			true
		);
	}
}

// void ADW_Mimic::Interact_Implementation(AActor* Interactor)
// {
// 	if (!bIsClosed) return;
//
// 	bIsClosed = false;
//
// 	if (AAIController* Ctr = Cast<AAIController>(GetController()))
// 	{
// 		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
// 		{
// 			BBC->SetValueAsBool(FName("bHasOpened"), true);
// 		}
// 	}
//
// 	UDW_MimicAnimInstance* MimicAnimInstance = Cast<UDW_MimicAnimInstance>(GetMesh()->GetAnimInstance());
// 	if (IsValid(MimicAnimInstance))
// 	{
// 		MimicAnimInstance->bHasOpened = true;
// 	}
// }

// float ADW_Mimic::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
// 	class AController* EventInstigator, AActor* DamageCauser)
// {
// 	if (bIsClosed) return 0;
// 		
// 	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
// 	
// 	return 0;
// }
