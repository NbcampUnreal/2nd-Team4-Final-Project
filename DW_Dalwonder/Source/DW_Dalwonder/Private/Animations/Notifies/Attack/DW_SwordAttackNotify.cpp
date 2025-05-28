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

	// Trace 포인트
	const FVector CurrStart = CharacterWeapon->SwordTraceStartPoint->GetComponentLocation();
	const FVector CurrEnd   = CharacterWeapon->SwordTraceEndPoint->GetComponentLocation();

	// 이전 프레임 위치 미리 저장 안 되어있으면 초기화
	if (!bHasPrevTrace)
	{
		PrevTraceStart = CurrStart;
		PrevTraceEnd = CurrEnd;
		bHasPrevTrace = true;
	}

	// 여러 구간으로 나누기
	const int32 NumSteps = 5;
	for (int32 i = 0; i < NumSteps; ++i)
	{
		const float Alpha = static_cast<float>(i) / (NumSteps - 1);
		const FVector Prev = FMath::Lerp(PrevTraceStart, PrevTraceEnd, Alpha);
		const FVector Curr = FMath::Lerp(CurrStart, CurrEnd, Alpha);

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(PlayerCharacter);
		Params.AddIgnoredActor(CharacterWeapon);

		// 디버그 시각화
		DrawDebugLine(World, Prev, Curr, FColor::Red, false, 1.0f, 0, 1.5f);
		DrawDebugSphere(World, Curr, 5.f, 12, FColor::Yellow, false, 1.0f);

		if (World->LineTraceSingleByChannel(Hit, Prev, Curr, ECC_SwordTrace, Params))
		{
			AActor* HitActor = Hit.GetActor();
			if (!IsValid(HitActor)) continue;

			// 바닥 무시
			if (Hit.ImpactNormal.Z > 0.8f)
			{
				UE_LOG(LogTemp, Warning, TEXT("바닥 충돌 무시됨: %s"), *GetNameSafe(HitActor));
				continue;
			}

			// 인터페이스 판별
			if (HitActor->Implements<UBearableInterface>())
			{
				if (!IBearableInterface::Execute_CanBeCut(HitActor, Hit))
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

			// 공격 판정 적용
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
				UE_LOG(LogTemp, Warning, TEXT("[SwordTrace] Hit Actor: %s"), *GetNameSafe(HitActor));
			}
		}
	}

	// 다음 틱을 위해 위치 갱신
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
	}
}
