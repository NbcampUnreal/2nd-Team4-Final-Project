// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Skeleton/DW_SkeletonBoss.h"

#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DW_CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Projectile/ProjectileSpawnerComponent.h"


// Sets default values
ADW_SkeletonBoss::ADW_SkeletonBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LeftHandWeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandWeapon"));
	LeftHandWeaponComp->SetupAttachment(GetMesh(), TEXT("hand_l"));

	RightHandWeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandWeapon"));
	RightHandWeaponComp->SetupAttachment(GetMesh(), TEXT("hand_r"));

	TraceStart->SetupAttachment(GetMesh(), TEXT("hand_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("hand_r"));

	PJSpawner = CreateDefaultSubobject<UProjectileSpawnerComponent>(TEXT("PJSpawner"));
	PJSpawner->SetupAttachment(GetMesh(), TEXT("hand_l"));

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

// Called when the game starts or when spawned
void ADW_SkeletonBoss::BeginPlay()
{
	Super::BeginPlay();

	ChangeFormTypeByRandom();
}

void ADW_SkeletonBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotateToPlayer)
	{
		if (PlayerCharacter)
		{
			FRotator CurrentRotation = GetActorRotation();

			FVector DirectionToPlayer = PlayerCharacter->GetActorLocation() - GetActorLocation();
			DirectionToPlayer.Z = 0;

			if (!DirectionToPlayer.IsNearlyZero())
			{
				FRotator TargetRotation = DirectionToPlayer.Rotation();
				FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.f);

				SetActorRotation(NewRotation);
			}
		}
	}
}

void ADW_SkeletonBoss::ChangeFormTypeByRandom()
{
	ESkeletonFormType NewFormType;
	
	do
	{
		constexpr int32 MaxEnumValue = static_cast<int32>(ESkeletonFormType::Max);
		int32 RandomIndex = FMath::RandRange(0, MaxEnumValue - 1);

		NewFormType = static_cast<ESkeletonFormType>(RandomIndex);
	}
	while (NewFormType == CurrentFormType);
	
	CurrentFormType = NewFormType;
	ChangeFormType(CurrentFormType);
}

void ADW_SkeletonBoss::ChangeFormType(const ESkeletonFormType FormType)
{

	CurrentFormType = FormType;
	
	if (const FFormData* FormData = FormDataMap.Find(CurrentFormType))
	{
		// 구토
		if (FormData->SkeletalMesh)
		{
			GetMesh()->SetSkeletalMesh(FormData->SkeletalMesh);
		}

		// ABP
		if (FormData->AnimBP)
		{
			GetMesh()->SetAnimInstanceClass(FormData->AnimBP);
		}

		// 무기(왼손)
		if (LeftHandWeaponComp && FormData->LeftHandWeapon)
		{
			LeftHandWeaponComp->SetSkeletalMesh(FormData->LeftHandWeapon);
			// LeftHandWeaponComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_l"));
		}
		else
		{
			LeftHandWeaponComp->SetSkeletalMesh(nullptr);
		}

		// 무기(오른손)
		if (RightHandWeaponComp && FormData->RightHandWeapon)
		{
			RightHandWeaponComp->SetSkeletalMesh(FormData->RightHandWeapon);
			// RightHandWeaponComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r"));
		}
		else
		{
			RightHandWeaponComp->SetSkeletalMesh(nullptr);
		}

		// 몽타주
		AnimMontages = FormData->Montages;
	}

	if (AAIController* Ctr = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
		{
			BBC->SetValueAsEnum(FName("BossForm"), static_cast<uint8>(FormType));
		}
	}
	
	if (IsValid(ChangeFormNS))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ChangeFormNS,
		GetActorLocation(),
		GetActorRotation(),
		FVector(1.f),
		true,
		true);
	}
}

void ADW_SkeletonBoss::JumpToTarget(const FVector& TargetLocation, float AirTime)
{
	FVector StartLocation = GetActorLocation();
	FVector Delta = TargetLocation - StartLocation;

	if (FMath::IsNearlyZero(Delta.Size()) || AirTime <= 0.f)
		return;

	const float Gravity = GetCharacterMovement()->GetGravityZ();
	if (FMath::IsNearlyZero(Gravity))
		return;

	FVector HorizontalDelta = FVector(Delta.X, Delta.Y, 0.f);
	float HorizontalDistance = HorizontalDelta.Size();
	FVector HorizontalVelocity = HorizontalDelta / AirTime;

	float VerticalVelocity = (Delta.Z - 0.5f * Gravity * AirTime * AirTime) / AirTime;
	FVector LaunchVelocity = HorizontalVelocity + FVector(0.f, 0.f, VerticalVelocity);

	LaunchCharacter(LaunchVelocity, true, true);
}

void ADW_SkeletonBoss::SpawnTeleportNS()
{
	if (IsValid(TeleportNS))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		TeleportNS,
		GetActorLocation(),
		GetActorRotation(),
		FVector(1.f),
		true,
		true);
	}
}

void ADW_SkeletonBoss::Dead()
{
	Super::Dead();

	SetActorTickEnabled(false);
	GetCharacterMovement()->DisableMovement();

	FTimerHandle RagdollTimerHandle;
	GetWorldTimerManager().SetTimer(RagdollTimerHandle, this, &ADW_SkeletonBoss::DoRagDoll, 2.5f, false);
	
}

void ADW_SkeletonBoss::DoRagDoll()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetSimulatePhysics(true);
}
