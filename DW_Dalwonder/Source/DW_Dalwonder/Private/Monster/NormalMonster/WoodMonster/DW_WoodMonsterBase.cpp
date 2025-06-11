#include "Monster/NormalMonster/WoodMonster/DW_WoodMonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Character/DW_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"


ADW_WoodMonsterBase::ADW_WoodMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnCollision->SetupAttachment(GetCapsuleComponent());
	SpawnCollision->SetCollisionProfileName("NoCollision");
	SpawnCollision->InitBoxExtent(FVector(500.f, 500.f, 10.f));

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("index_01_l"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("index_01_r"));
}

float ADW_WoodMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (MonsterHP <= 0)
	{
		return 0;
	}

	if (MonsterHP <= SpawnHP && !bIsSpawned)
	{
		SpawnToWoodGolem();
	}

    return 0.0f;
}

void ADW_WoodMonsterBase::SpawnToWoodGolem()
{
	if (!GetWorld()) return;

	bIsSpawned = true;

	if (bUseRandomAmount)
	{
		SpawnAmount = FMath::RandRange(1, SpawnAmount);
	}

	for (int32 i = 0; i < SpawnAmount; ++i)
	{
		if (SpawningClass)
		{
			FTransform SpawnLocation = GetRandomSpawnPoint();
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawningClass, SpawnLocation, SpawnParams);
			if (SpawnedActor)
			{
				UE_LOG(LogTemp, Error, TEXT("스폰 실패! 위치: %s"), *SpawnLocation.GetLocation().ToString());
			}
		}
	}
}

void ADW_WoodMonsterBase::DeadLogic()
{
	SetActorTickEnabled(false);
	GetCharacterMovement()->DisableMovement();

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	FTimerHandle DestroyDelayTimer;
	GetWorldTimerManager().SetTimer(DestroyDelayTimer, this, &ADW_NormalMonsterBase::DestroyDissolve, DestroyTime, false);

}

FTransform ADW_WoodMonsterBase::GetRandomSpawnPoint()
{
	FVector Extent = SpawnCollision->GetScaledBoxExtent();
	FVector Origin = GetActorLocation();

	FVector RandomXY = FVector(
		FMath::FRandRange(-Extent.X, Extent.X),
		FMath::FRandRange(-Extent.Y, Extent.Y),
		0.f
	);

	FVector CandidatePoint = Origin + RandomXY;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation ProjectedLocation;

	bool bFoundNavPoint = NavSys && NavSys->ProjectPointToNavigation(
		CandidatePoint,
		ProjectedLocation,
		FVector(500.f, 500.f, 1000.f)
	);

	FVector FinalPoint = ProjectedLocation.Location;

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(FinalPoint, GetPlayerCharacter()->GetActorLocation());
	LookAtRotation.Pitch = 0.f; 
	LookAtRotation.Roll = 0.f;

	return FTransform(LookAtRotation, FinalPoint, FVector(1.f, 1.f, 1.f));
}
