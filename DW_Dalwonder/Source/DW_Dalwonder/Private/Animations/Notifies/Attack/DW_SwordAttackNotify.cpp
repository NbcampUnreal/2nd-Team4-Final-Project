#define ECC_SwordTrace ECC_GameTraceChannel1

#include "Animations/Notifies/Attack/DW_SwordAttackNotify.h"
#include "DrawDebugHelpers.h"
#include "Character/DW_Warrior.h"
#include "Interface/BearableInterface.h"
#include "Character/DW_SwordBase.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ItemBase.h"
#include "Character/CharacterArmorComponent.h"
#include "Character/CharacterStatComponent.h"

UDW_SwordAttackNotify::UDW_SwordAttackNotify()
{
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
	const FVector CurrCancelEnd = CharacterWeapon->SwordTraceCancelPoint->GetComponentLocation();
	const FVector CurrEnd = CharacterWeapon->SwordTraceEndPoint->GetComponentLocation();

	if (!bHasPrevTrace)
	{
		PrevTraceStart = CurrStart;
		PrevTraceCancel = CurrCancelEnd;
		PrevTraceEnd = CurrEnd;
		bHasPrevTrace = true;
	}

	const int32 NumSteps = 5;
	const float SphereRadius = 5.f;
	FCollisionShape SweepShape = FCollisionShape::MakeSphere(SphereRadius);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(SwordSweep), false);
	Params.AddIgnoredActor(PlayerCharacter);
	Params.AddIgnoredActor(CharacterWeapon);

	// Cancel용 영역 검사 (칼 중간까지만)
	for (int32 i = 0; i < NumSteps; ++i)
	{
		const float Alpha = static_cast<float>(i) / (NumSteps - 1);
		const FVector Start = FMath::Lerp(PrevTraceStart, PrevTraceCancel, Alpha);
		const FVector End = FMath::Lerp(CurrStart, CurrCancelEnd, Alpha);
	
		TArray<FHitResult> CancelHits;
		if (World->SweepMultiByChannel(CancelHits, Start, End, FQuat::Identity, ECC_SwordTrace, SweepShape, Params))
		{
			for (const FHitResult& Hit : CancelHits)
			{
				AActor* HitActor = Hit.GetActor();
				if (!IsValid(HitActor)) continue;

				if (FMath::Abs(Hit.ImpactNormal.Z) >= 0.65f)
				{
					continue;
				}

				if (HitActor->Implements<UBearableInterface>())
				{
					if (!IBearableInterface::Execute_CanBeCut(HitActor, Hit))
					{
						PlayerCharacter->CancelAttack();
						return;
					}
					continue;
				}

				if (Hit.bBlockingHit)
				{
					PlayerCharacter->CancelAttack();
					return;
				}
			}
		}
	}
	
	// 데미지 영역 검사 (칼 전체)
	for (int32 i = 0; i < NumSteps; ++i)
	{
		const float Alpha = static_cast<float>(i) / (NumSteps - 1);
		const FVector Start = FMath::Lerp(PrevTraceStart, PrevTraceEnd, Alpha);
		const FVector End = FMath::Lerp(CurrStart, CurrEnd, Alpha);

		TArray<FHitResult> HitResults;
		if (World->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_SwordTrace, SweepShape, Params))
		{
			for (const FHitResult& Hit : HitResults)
			{
				AActor* HitActor = Hit.GetActor();
				if (!IsValid(HitActor)) continue;

				if (PlayerCharacter->AttackingActors.Contains(HitActor)) continue;
				PlayerCharacter->AttackingActors.Add(HitActor);

				// 기본 공격력
				float TotalDamage = PlayerCharacter->GetCharacterStatComponent()->GetTotalAttack() * AttackMultiplier;

				// Attribute에서 보너스 데미지 적용
				if (UDW_AttributeComponent* Attr = PlayerCharacter->FindComponentByClass<UDW_AttributeComponent>())
				{
					// 무기 타입 기반 추가 피해
					if (UCharacterArmorComponent* ArmorComp = PlayerCharacter->FindComponentByClass<UCharacterArmorComponent>())
					{
						if (ArmorComp->Weapon)
						{
							FString ItemCodeStr = ArmorComp->Weapon->ItemCode; // ex) "0011", "00112", "00124"
							if (ItemCodeStr.Len() > 3) // 최소 길이만 확인 (안전하게)
							{
								FString WeaponCodeStr = ItemCodeStr.Mid(3); // 앞 3자리 제거 → "1", "12", "24" 등
								int32 WeaponCode = FCString::Atoi(*WeaponCodeStr); // "1" → 1

								// 대검: 1~11, 23, 24
								if ((WeaponCode >= 1 && WeaponCode <= 11) || WeaponCode == 23 || WeaponCode == 24)
								{
									TotalDamage += Attr->BonusGreatswordDamageMod;
								}
								// 장검: 12~22, 25, 26
								else if ((WeaponCode >= 12 && WeaponCode <= 22) || WeaponCode == 25 || WeaponCode == 26)
								{
									TotalDamage += Attr->BonusLongswordDamageMod;
								}
							}
						}
					}

					// 몬스터 타입 판정
					if (HitActor->ActorHasTag("NormalMonster"))
					{
						TotalDamage += Attr->BonusDamageToNormalEnemies;
					}
					else if (HitActor->ActorHasTag("BossMonster"))
					{
						TotalDamage += Attr->BonusDamageToBoss;
					}
				}

				// 원래 AttackDamage로 되어있었습니다 데미지 계산값 들어가도록 바꿈
				UGameplayStatics::ApplyPointDamage(
					HitActor,
					TotalDamage,
					(Hit.TraceEnd - Hit.TraceStart).GetSafeNormal(),
					Hit,
					PlayerCharacter->GetController(),
					PlayerCharacter,
					UDamageType::StaticClass()
				);
			}
		}
	}

	PrevTraceStart = CurrStart;
	PrevTraceCancel = CurrCancelEnd;
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
