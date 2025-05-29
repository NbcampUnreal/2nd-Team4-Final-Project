// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Sevarog/DW_Sevarog.h"

#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/MonsterStatsTable.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"


// Sets default values
ADW_Sevarog::ADW_Sevarog()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//부착 후 에디터에서 위치 세부 조정 필요
	TraceStart->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	
	Hammer = CreateDefaultSubobject<USceneComponent>("Hammer");
	Hammer->SetupAttachment(GetMesh(), TEXT("weapon_r"));
}

float ADW_Sevarog::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentPhase == 1 && MonsterHP * 2 <= MonsterMaxHP)
	{
		SetCurrentPhase(2);
	}
	
	return 0;
}

void ADW_Sevarog::AirAttack()
{
	FVector HammerLocation = Hammer->GetComponentLocation();
	float Radius = 250.0f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		HammerLocation,
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
				UGameplayStatics::ApplyDamage(HitActor, MonsterDamage, GetController(), this, UDamageType::StaticClass());
			}
		}
	}

	DrawDebugSphere(GetWorld(), HammerLocation, Radius, 16, FColor::Red, false, 1.0f);

	if (!AirAttackNS) return;

	FVector SpawnLocation = Hammer->GetComponentLocation();
	FRotator SpawnRotation = GetActorRotation();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		AirAttackNS,
		SpawnLocation,
		SpawnRotation,
		FVector(2.f),
		true,
		true
	);
}

void ADW_Sevarog::DoTeleport()
{
	if (IsValid(TeleportMontage))
	{
		UAnimMontage* Montage = TeleportMontage;
		
		if (Montage && GetMesh())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(Montage);
		}
	}
}

void ADW_Sevarog::DoRangedTeleport()
{
	if (IsValid(RangedTeleportMontage))
	{
		UAnimMontage* Montage = RangedTeleportMontage;
		
		if (Montage && GetMesh())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(Montage);
		}
	}
}

void ADW_Sevarog::SpawnMonster(const TSubclassOf<ADW_MonsterBase>& SpawnMob) const
{
	const FVector RandomOffset = FVector(
	FMath::RandRange(-500.f, 500.f),
	FMath::RandRange(-500.f, 500.f),
	0.f);

	const FVector SpawnLocation = GetActorLocation() + RandomOffset;

	if (IsValid(SpawnMonsterNS))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		SpawnMonsterNS,
		SpawnLocation,
		GetActorRotation(),
		FVector(1.f),
		true,
		true);
	}

	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(SpawnMob, SpawnLocation, GetActorRotation(), SpawnParams);

}
