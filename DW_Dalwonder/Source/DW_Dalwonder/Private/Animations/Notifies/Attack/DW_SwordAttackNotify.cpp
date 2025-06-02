#define ECC_SwordTrace ECC_GameTraceChannel1

#include "Animations/Notifies/Attack/DW_SwordAttackNotify.h"
#include "DrawDebugHelpers.h"
#include "Character/DW_Warrior.h"
#include "Interface/BearableInterface.h"
#include "Character/DW_SwordBase.h"
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

	bHasPrevTrace = false;
	
	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner()))
	{
		PlayerCharacter = Cast<ADW_Warrior>(MeshComp->GetOwner());

		if (IsValid(PlayerCharacter))
		{
			CharacterWeapon = Cast<ADW_SwordBase>(PlayerCharacter->GetWeapon());
		}
	}
}

void UDW_SwordAttackNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(PlayerCharacter) || !IsValid(CharacterWeapon)) return;
	UWorld* World = MeshComp->GetWorld();
	if (!IsValid(World)) return;

	const FVector CurrStart = CharacterWeapon->SwordTraceStartPoint->GetComponentLocation();
	const FVector CurrEnd   = CharacterWeapon->SwordTraceEndPoint->GetComponentLocation();

	if (!bHasPrevTrace)
	{
		PrevTraceStart = CurrStart;
		PrevTraceEnd = CurrEnd;
		bHasPrevTrace = true;
	}

	const int32 NumSteps = 5;
	const float SphereRadius = 5.f;
	FCollisionShape SweepShape = FCollisionShape::MakeSphere(SphereRadius);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(SwordSweep), false);
	Params.AddIgnoredActor(PlayerCharacter);
	Params.AddIgnoredActor(CharacterWeapon);

	for (int32 i = 0; i < NumSteps; ++i)
	{
		const float Alpha = static_cast<float>(i) / (NumSteps - 1);
		const FVector Start = FMath::Lerp(PrevTraceStart, PrevTraceEnd, Alpha);
		const FVector End   = FMath::Lerp(CurrStart, CurrEnd, Alpha);

		DrawDebugCapsule(
			World,
			(Start + End) * 0.5f,
			(End - Start).Size() * 0.5f,
			SphereRadius,
			FRotationMatrix::MakeFromZ(End - Start).ToQuat(),
			FColor::Green,
			false, 0.2f, 0, 1.f
		);
		
		TArray<FHitResult> HitResults;
		if (World->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_SwordTrace, SweepShape, Params))
		{
			for (const FHitResult& Hit : HitResults)
			{
				AActor* HitActor = Hit.GetActor();
				if (!IsValid(HitActor)) continue;

				// ✅ 이전에 무시된 액터인지 확인
				if (IgnoredActors.Contains(HitActor))
				{
					continue;
				}

				// ✅ 바닥이면 무시 리스트에 등록
				if (Hit.ImpactNormal.Z > 0.8f)
				{
					IgnoredActors.Add(HitActor);
					continue;
				}

				// ✅ CanBeCut 검사
				if (HitActor->Implements<UBearableInterface>())
				{
					if (!IBearableInterface::Execute_CanBeCut(HitActor, Hit))
					{
						PlayerCharacter->CancelAttack();
						return;
					}
				}
				else
				{
					PlayerCharacter->CancelAttack();
					return;
				}

				// 데미지 적용 (중복 방지)
				if (!PlayerCharacter->AttackingActors.Contains(HitActor))
				{
					PlayerCharacter->AttackingActors.Add(HitActor);

					UGameplayStatics::ApplyDamage(
						HitActor,
						AttackDamage,
						PlayerCharacter->GetController(),
						PlayerCharacter,
						UDamageType::StaticClass()
					);

					UE_LOG(LogTemp, Warning, TEXT("[Trace] ✅ 데미지 적용: %s | Normal.Z: %.2f"),
						*GetNameSafe(HitActor),
						Hit.ImpactNormal.Z);
				}
			}
		}
	}

	PrevTraceStart = CurrStart;
	PrevTraceEnd = CurrEnd;
}


void UDW_SwordAttackNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->AttackingActors.Empty();
		IgnoredActors.Empty();
	}
}
