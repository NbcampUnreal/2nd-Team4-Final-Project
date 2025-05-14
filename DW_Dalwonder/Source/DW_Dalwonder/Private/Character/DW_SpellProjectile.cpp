#include "Character/DW_SpellProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ADW_SpellProjectile::ADW_SpellProjectile()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(ProjectileCollision);
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
	SpellNiagara = CreateDefaultSubobject<UNiagaraSystem>(TEXT("SpellNiagara"));

	ProjectileCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetupAttachment(Root);
	
	ProjectileTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStartPoint"));
	ProjectileTraceStartPoint->SetupAttachment(Root);
	ProjectileTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEndPoint"));
	ProjectileTraceEndPoint->SetupAttachment(Root);

	InitialLifeSpan = 2.f;
	ProjectileCollision->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	

	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ADW_SpellProjectile::OnProjectileBeginOverlap);
}

void ADW_SpellProjectile::FireInDirection(const FVector& Direction)
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
}

void ADW_SpellProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ADW_SpellProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = ProjectileTraceStartPoint->GetComponentLocation();
	const FVector End = ProjectileTraceEndPoint->GetComponentLocation();

	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	IgnoreActors.Add(this->GetOwner());
	
	UKismetSystemLibrary::CapsuleTraceMulti(this, Start, End,
		ProjectileCollision->GetScaledCapsuleRadius(),
		ProjectileCollision->GetScaledCapsuleHalfHeight(),
		TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true);

	for (FHitResult HitResult : HitResults)
	{
		if (IsValid(HitResult.GetActor()) == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!"));
		}
	}
}
