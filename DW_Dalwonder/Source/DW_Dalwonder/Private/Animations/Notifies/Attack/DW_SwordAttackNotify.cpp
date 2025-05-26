#include "Animations/Notifies/Attack/DW_SwordAttackNotify.h"
#include "DrawDebugHelpers.h"
#include "Character/DW_Warrior.h"
#include "Character/DW_Sword.h"
#include "Kismet/GameplayStatics.h"

UDW_SwordAttackNotify::UDW_SwordAttackNotify()
{
	AttackDamage = 10.f;
	PlayerCharacter = nullptr;
	CharacterWeapon = nullptr;
}

void UDW_SwordAttackNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner()))
	{
		PlayerCharacter = Cast<ADW_Warrior>(MeshComp->GetOwner());

		if (IsValid(PlayerCharacter))
		{
			CharacterWeapon = Cast<ADW_Sword>(PlayerCharacter->GetWeapon());
		}
	}
}

void UDW_SwordAttackNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	UWorld* World = MeshComp->GetWorld();

	if (IsValid(PlayerCharacter) && IsValid(CharacterWeapon))
	{
		const FVector TraceStart = CharacterWeapon->SwordTraceStartPoint->GetComponentLocation();
		const FVector TraceEnd = CharacterWeapon->SwordTraceEndPoint->GetComponentLocation();
	
		TArray<FHitResult> HitResults;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(PlayerCharacter);
		Params.AddIgnoredActor(CharacterWeapon);
		
		DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 1.f, -1, 2.f);
		DrawDebugSphere(World, TraceStart, 5.f, 12, FColor::Yellow, false, 1.f);
		DrawDebugSphere(World, TraceEnd, 5.f, 12, FColor::Yellow, false, 1.f);

		World->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECC_Pawn, Params);
		for (FHitResult& HitResult : HitResults)
		{
			if (AActor* HitActor = HitResult.GetActor())
			{
				PlayerCharacter->AttackingActors.Add(HitActor);
				UGameplayStatics::ApplyDamage(HitActor, AttackDamage, PlayerCharacter->GetController(), PlayerCharacter, UDamageType::StaticClass());
			}
		}
	}
}

void UDW_SwordAttackNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->AttackingActors.Empty();
	}
}
