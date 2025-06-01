// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Sevarog/DW_Sevarog.h"

#include "AIController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DW_CharacterBase.h"
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

	TrailNS = CreateDefaultSubobject<UNiagaraComponent>("TrailParticle");
	TrailNS->SetupAttachment(RootComponent);

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

void ADW_Sevarog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter)
	{
		FRotator CurrentRotation = GetActorRotation();

		FVector DirectionToPlayer = PlayerCharacter->GetActorLocation() - GetActorLocation();
		DirectionToPlayer.Z = 0;

		if (!DirectionToPlayer.IsNearlyZero())
		{
			FRotator TargetRotation = DirectionToPlayer.Rotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 1.5f);

			SetActorRotation(NewRotation);
		}
	
	}
}

void ADW_Sevarog::AirAttack()
{
	const FVector HammerLocation = Hammer->GetComponentLocation();
	constexpr float Radius = 200.0f;

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
				UGameplayStatics::ApplyDamage(HitActor, MonsterDamage * MonsterDamageMultiplier, GetController(), this, UDamageType::StaticClass());
				const FVector KnockBackDirection = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				constexpr float KnockBackStrength = 2000.f;

				PlayerCharacter->KnockBackCharacter(KnockBackDirection, KnockBackStrength, true);
			}
		}
	}

	DrawDebugSphere(GetWorld(), HammerLocation, Radius, 16, FColor::Red, false, 1.0f);

	if (!AirAttackNS) return;

	const FVector SpawnLocation = Hammer->GetComponentLocation();
	const FRotator SpawnRotation = GetActorRotation();

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

void ADW_Sevarog::DoTeleport() const
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

void ADW_Sevarog::DoRangedTeleport() const
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

void ADW_Sevarog::SurroundedAttack()
{
	const FVector HammerLocation = GetActorLocation() - FVector(0,0,150.f);
	constexpr float Radius = 350.0f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const bool bHit = GetWorld()->OverlapMultiByChannel(
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
				UGameplayStatics::ApplyDamage(HitActor, MonsterDamage * MonsterDamageMultiplier, GetController(), this, UDamageType::StaticClass());
				const FVector KnockBackDirection = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				constexpr float KnockBackStrength = 1000.f;

				PlayerCharacter->KnockBackCharacter(KnockBackDirection, KnockBackStrength, false);
			}
		}
	}

	if (bDrawDebugTrace)
	{
		DrawDebugSphere(GetWorld(), HammerLocation, Radius, 16, FColor::Red, false, 1.0f);
	}

	if (!SurroundedAttackNS) return;

	const FVector SpawnLocation = GetActorLocation() - FVector(0,0,150.f);
	const FRotator SpawnRotation = GetActorRotation();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		SurroundedAttackNS,
		SpawnLocation,
		SpawnRotation,
		FVector(1.f),
		true,
		true
	);
}

void ADW_Sevarog::BoxAttack()
{
	const FVector LocalOffset = FVector(300.f, 0.f, -200.f);
	const FVector BoxExtent = FVector(300.f, 150.f, 200.f);
	
	const FVector BoxCenter = GetActorLocation() + GetActorRotation().RotateVector(LocalOffset);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		BoxCenter,
		GetActorQuat(),
		ECC_Pawn,
		FCollisionShape::MakeBox(BoxExtent),
		QueryParams
	);

	if (bHit)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();
			if (HitActor && HitActor->ActorHasTag("Player"))
			{
				UGameplayStatics::ApplyDamage(HitActor, MonsterDamage * MonsterDamageMultiplier, GetController(), this, UDamageType::StaticClass());
				const FVector KnockBackDirection = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				constexpr float KnockBackStrength = 1000.f;

				PlayerCharacter->KnockBackCharacter(KnockBackDirection, KnockBackStrength, false);
			}
		}
	}

	if (bDrawDebugTrace)
	{
		DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, GetActorQuat(), FColor::Red, false, 1.0f);
	}


	if (!BoxAttackNS) return;

	const FVector EffectOffset = FVector(50.f, 0.f, -200.f);
	const FVector EffectLocation = GetActorLocation() + GetActorRotation().RotateVector(EffectOffset);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		BoxAttackNS,
		EffectLocation,
		GetActorRotation(),
		FVector(1.f),
		true,
		true
	);
}

void ADW_Sevarog::Dead()
{
	Super::Dead();

	if (!bIsRealBoss)
	{
		if (!RepDeadNS) return;

		const FVector SpawnLocation = GetActorLocation();
		const FRotator SpawnRotation = GetActorRotation();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			RepDeadNS,
			SpawnLocation,
			SpawnRotation,
			FVector(1.f),
			true,
			true
		);

		Destroy();
	}
}
