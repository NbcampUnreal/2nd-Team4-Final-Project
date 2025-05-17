#include "Animations/Notifies/Attack/DW_SwordAttackNotify.h"
#include "DrawDebugHelpers.h"
#include "Character/DW_Warrior.h"
#include "Character/DW_Sword.h"

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
		AActor* Owner = MeshComp->GetOwner();
		PlayerCharacter = Cast<ADW_Warrior>(Owner);

		if (IsValid(PlayerCharacter))
		{
			CharacterWeapon = Cast<ADW_Sword>(PlayerCharacter->GetWeapon());
			if (IsValid(CharacterWeapon))
			{
				PrevTraceStart = CharacterWeapon->SwordTraceStartPoint->GetComponentLocation();
				PrevTraceEnd = CharacterWeapon->SwordTraceEndPoint->GetComponentLocation();
			}
		}
	}
}

void UDW_SwordAttackNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (IsValid(PlayerCharacter))
	{
		if (IsValid(CharacterWeapon))
		{
			const FVector CurTraceStart = CharacterWeapon->SwordTraceStartPoint->GetComponentLocation();
			const FVector CurTraceEnd = CharacterWeapon->SwordTraceEndPoint->GetComponentLocation();

			const int NumSteps = 5;
			for (int i = 0; i < NumSteps; ++i)
			{
				float Alpha = static_cast<float>(i) / (NumSteps - 1);
				FVector Prev = FMath::Lerp(PrevTraceStart, PrevTraceEnd, Alpha);
				FVector Curr = FMath::Lerp(CurTraceStart, CurTraceEnd, Alpha);

				FHitResult Hit;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(PlayerCharacter);
				
				DrawDebugLine(GetWorld(), Prev, Curr, FColor::Red, false, 1.f, 0, 2.f);
				DrawDebugSphere(GetWorld(), Curr, 5.f, 12, FColor::Yellow, false, 1.f);

				if (GetWorld()->LineTraceSingleByChannel(Hit, Prev, Curr, ECC_Pawn, Params))
				{
					if (AActor* HitActor = Hit.GetActor())
					{
						PlayerCharacter->AttackingActors.Add(HitActor);
					}
				}
			}
			
			PrevTraceStart = CharacterWeapon->SwordTraceStartPoint->GetComponentLocation();
			PrevTraceEnd = CharacterWeapon->SwordTraceEndPoint->GetComponentLocation();
		}
	}
}

void UDW_SwordAttackNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner()))
	{
		AActor* Owner = MeshComp->GetOwner();
		ADW_CharacterBase* Character = Cast<ADW_CharacterBase>(Owner);

		if (IsValid(Character))
		{
			Character->AttackEnemy(AttackDamage);
		}
	}
}
