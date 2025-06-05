#include "Monster/NormalMonster/WoodMonster/DW_WoodMonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Character/DW_CharacterBase.h"
#include "Monster/NormalMonster/WoodMonster/Mob_WoodGolem.h"


ADW_WoodMonsterBase::ADW_WoodMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnCollision->SetupAttachment(GetCapsuleComponent());
	SpawnCollision->SetCollisionProfileName("NoCollision");

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("index_01_l"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("index_01_r"));
}

float ADW_WoodMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

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
		if (UClass* SpawningClass = AMob_WoodGolem::StaticClass())
		{
			FTransform SpawnLocation = GetRandomSpawnPoint();
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			if (SpawningClass == nullptr) return;

			GetWorld()->SpawnActor<AActor>(SpawningClass, SpawnLocation, SpawnParams);
		}
	}
}

FTransform ADW_WoodMonsterBase::GetRandomSpawnPoint()
{
	FVector Extent = SpawnCollision->GetScaledBoxExtent();
	FVector Origin = SpawnCollision->GetComponentLocation();

	FVector PointLocation = Origin + FVector(
		FMath::FRandRange(-Extent.X, Extent.X),
		FMath::FRandRange(-Extent.Y, Extent.Y),
		Origin.Z
	);

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PointLocation, GetPlayerCharacter()->GetActorLocation());
	LookAtRotation.Pitch = 0.f; 
	LookAtRotation.Roll = 0.f;

	return FTransform(LookAtRotation, PointLocation, FVector(1.f, 1.f, 1.f));
}
