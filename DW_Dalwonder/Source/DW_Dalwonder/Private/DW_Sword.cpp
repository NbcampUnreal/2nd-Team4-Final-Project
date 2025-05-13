#include "DW_Sword.h"
#include "Monster/DW_MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"

ADW_Sword::ADW_Sword()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Root);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(Root);
	
	SwordTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStartPoint"));
	SwordTraceStartPoint->SetupAttachment(Root);
	SwordTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEndPoint"));
	SwordTraceEndPoint->SetupAttachment(Root);

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ADW_Sword::OnSwordBeginOverlap);
}

void ADW_Sword::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADW_Sword::OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = SwordTraceStartPoint->GetComponentLocation();
	const FVector End = SwordTraceEndPoint->GetComponentLocation();

	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	IgnoreActors.Add(this->GetOwner());
	
	UKismetSystemLibrary::CapsuleTraceMulti(this, Start, End,
		WeaponCollision->GetScaledCapsuleRadius(),
		WeaponCollision->GetScaledCapsuleHalfHeight(),
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
			ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(HitResult.GetActor());
			if (IsValid(Monster) == true)
			{
				//*@TODO : 여기 부분 나중에 자세히 수정할 것 */
				FDamageEvent DamageEvent;
				Monster->TakeDamage(10.f, DamageEvent, this->GetOwner()->GetInstigatorController(), this);
			}
		}
	}
}
