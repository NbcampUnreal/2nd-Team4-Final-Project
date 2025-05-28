#define ECC_SwordTrace ECC_GameTraceChannel1

#include "Animations/Notifies/Attack/DW_SwordAttackNotify.h"
#include "DrawDebugHelpers.h"
#include "Character/DW_Warrior.h"
#include "Character/DW_Sword.h"
#include "Interface/BearableInterface.h"
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

	const FVector TraceStart = CharacterWeapon->SwordTraceStartPoint->GetComponentLocation();
	const FVector TraceEnd   = CharacterWeapon->SwordTraceEndPoint->GetComponentLocation();

	// ✅ 캡슐 판정 설정
	const float CapsuleRadius = CharacterWeapon->CapsuleTraceRadius;
	const float CapsuleHalfHeight = CharacterWeapon->CapsuleTraceHalfHeight;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerCharacter);
	Params.AddIgnoredActor(CharacterWeapon);

	// 디버그 시각화
	DrawDebugCapsule(World, (TraceStart + TraceEnd) * 0.5f, CapsuleHalfHeight, CapsuleRadius,
		FQuat::FindBetweenNormals(FVector::UpVector, TraceEnd - TraceStart), FColor::Green, false, 1.0f);

	UKismetSystemLibrary::CapsuleTraceMulti(
		World,
		TraceStart,
		TraceEnd,
		CapsuleRadius,
		CapsuleHalfHeight,
		UEngineTypes::ConvertToTraceType(ECC_SwordTrace), // ✅ 커스텀 채널 사용
		false,
		{ PlayerCharacter, CharacterWeapon },
		EDrawDebugTrace::ForOneFrame,
		HitResults,
		true
	);
	// ✅ 아무 것도 안 맞았으면 공격 유지
	UE_LOG(LogTemp, Warning, TEXT("[SwordTrace] Hit Count: %d"), HitResults.Num());
	if (HitResults.Num() == 0)
	{
		return;
	}

	for (FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("[SwordTrace] Hit Actor: %s"), *GetNameSafe(HitActor));
		if (!HitActor) continue;

		if (HitResult.ImpactNormal.Z > 0.8f)
		{
			UE_LOG(LogTemp, Warning, TEXT("바닥 충돌 무시됨: %s"), *GetNameSafe(HitActor));
			continue;
		}

		// ✅ 베어질 수 있는지 판별
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

		// ✅ 공격 판정 적용
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
