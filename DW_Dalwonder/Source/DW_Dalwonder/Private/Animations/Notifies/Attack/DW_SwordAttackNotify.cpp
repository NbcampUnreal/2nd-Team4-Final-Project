#define ECC_SwordTrace ECC_GameTraceChannel1

#include "Animations/Notifies/Attack/DW_SwordAttackNotify.h"
#include "DrawDebugHelpers.h"
#include "Character/DW_Warrior.h"
#include "Character/DW_Sword.h"
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

	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner()))
	{
		PlayerCharacter = Cast<ADW_Warrior>(MeshComp->GetOwner());

		if (IsValid(PlayerCharacter))
		{
			CharacterWeapon = Cast<ADW_SwordBase>(PlayerCharacter->GetWeapon());
		}
	}
}

// void UDW_SwordAttackNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
// 	const FAnimNotifyEventReference& EventReference)
// {
// 	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
//
// 	UWorld* World = MeshComp->GetWorld();
//
// 	if (IsValid(PlayerCharacter) && IsValid(CharacterWeapon))
// 	{
// 		const FVector TraceStart = CharacterWeapon->SwordTraceStartPoint->GetComponentLocation();
// 		const FVector TraceEnd = CharacterWeapon->SwordTraceEndPoint->GetComponentLocation();
// 	
// 		TArray<FHitResult> HitResults;
// 		FCollisionQueryParams Params;
// 		Params.AddIgnoredActor(PlayerCharacter);
// 		Params.AddIgnoredActor(CharacterWeapon);
// 		
// 		DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 1.f, -1, 2.f);
// 		DrawDebugSphere(World, TraceStart, 5.f, 12, FColor::Yellow, false, 1.f);
// 		DrawDebugSphere(World, TraceEnd, 5.f, 12, FColor::Yellow, false, 1.f);
//
// 		World->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECC_Pawn, Params);
// 		for (FHitResult& HitResult : HitResults)
// 		{
// 			AActor* HitActor = HitResult.GetActor();
// 			if (!HitActor) continue;
//
// 			// ✅ 베어질 수 있는지 판별
// 			if (HitActor->Implements<UBearableInterface>())
// 			{
// 				const bool bCanCut = IBearableInterface::Execute_CanBeCut(HitActor, HitResult);
//
// 				if (!bCanCut)
// 				{
// 					// ❌ 베지 못함 → 공격 튕김 처리
// 					if (IsValid(PlayerCharacter))
// 					{
// 						PlayerCharacter->CancelAttack();
// 					}
//
// 					break; // 더 이상 공격 처리 안 함
// 				}
// 			}
// 			else
// 			{
// 				// ❌ 인터페이스 구현 안 된 애도 못 베게 함
// 				PlayerCharacter->CancelAttack();
// 				break;
// 			}
//
// 			// ✅ 성공적으로 공격 판정 적용
// 			PlayerCharacter->AttackingActors.Add(HitActor);
// 			UGameplayStatics::ApplyDamage(HitActor, AttackDamage, PlayerCharacter->GetController(), PlayerCharacter, UDamageType::StaticClass());
// 		}
//
// 	}
// }

void UDW_SwordAttackNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(PlayerCharacter) || !IsValid(CharacterWeapon)) return;

	UWorld* World = MeshComp->GetWorld();
	if (!IsValid(World)) return;

	const float CapsuleRadius = CharacterWeapon->CapsuleTraceRadius;
	const float CapsuleHalfHeight = CharacterWeapon->CapsuleTraceHalfHeight;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerCharacter);
	Params.AddIgnoredActor(CharacterWeapon);

	TArray<FHitResult> Hits;

	FVector TraceStart = CharacterWeapon->SwordTraceStartPoint->GetComponentLocation();
	FVector TraceEnd   = CharacterWeapon->SwordTraceEndPoint->GetComponentLocation();
	FVector TraceDir   = (TraceEnd - TraceStart).GetSafeNormal();
	FRotator CapsuleRotation = TraceDir.Rotation();

	// ✅ Sweep with rotation
	GetWorld()->SweepMultiByChannel(
		Hits,
		TraceStart,
		TraceEnd,
		CapsuleRotation.Quaternion(),
		ECC_SwordTrace,
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
		Params
	);

	// ✅ 디버그 시각화 추가
	DrawDebugLine(World, TraceStart, TraceEnd, FColor::Cyan, false, 1.0f, 0, 2.f);

	if (Hits.Num() == 0)
	{
		return;
	}

	for (FHitResult& HitResult : Hits)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor) continue;

		if (HitResult.ImpactNormal.Z > 0.8f)
		{
			UE_LOG(LogTemp, Warning, TEXT("바닥 충돌 무시됨: %s"), *GetNameSafe(HitActor));
			continue;
		}

		if (HitActor->Implements<UBearableInterface>())
		{
			if (!IBearableInterface::Execute_CanBeCut(HitActor, HitResult))
			{
				PlayerCharacter->CancelAttack();
				break;
			}
		}
		else
		{
			PlayerCharacter->CancelAttack();
			break;
		}

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
