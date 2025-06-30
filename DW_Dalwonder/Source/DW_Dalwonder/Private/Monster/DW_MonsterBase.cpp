#include "Monster/DW_MonsterBase.h"
#include "AIController.h"
#include "NavigationInvokerComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/DW_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Engine/DataTable.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/MonsterStatsTable.h"
#include "Sound/SoundBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/DamageEvents.h"
#include "Monster/MonsterDropTable.h"
#include "Item/ItemTranslator.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

struct FDropItemData;

ADW_MonsterBase::ADW_MonsterBase(): CurrentState(EMonsterState::Idle), DataTable(nullptr),
                                    AttackSoundComponent(nullptr), HitSoundComponent(nullptr), bIsAttacking(false), bCanParried(false),
                                    PlayerCharacter(nullptr), MonsterMaxHP(0),MonsterHP(0), MonsterDamage(0),
                                    MonsterSpeed(100), MonsterAccelSpeed(100), MonsterDamageMultiplier(1.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	AttackSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackSound"));
	AttackSoundComponent->SetupAttachment(RootComponent);
	AttackSoundComponent->bAutoActivate = false;
	AttackSoundComponent->SetVolumeMultiplier(0.7f);
	AttackSoundComponent->bOverrideAttenuation = true;
	AttackSoundComponent->AttenuationOverrides.bAttenuate = true;
	AttackSoundComponent->AttenuationOverrides.bSpatialize = true;
	AttackSoundComponent->AttenuationOverrides.AttenuationShape = EAttenuationShape::Sphere;
	AttackSoundComponent->AttenuationOverrides.AttenuationShapeExtents = FVector(200.f, 0.f, 0.f);
	AttackSoundComponent->AttenuationOverrides.FalloffDistance = 3000.0f;
	AttackSoundComponent->AttenuationOverrides.DistanceAlgorithm = EAttenuationDistanceModel::Linear;

	HitSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSound"));
	HitSoundComponent->SetupAttachment(RootComponent);
	HitSoundComponent->bAutoActivate = false;
	HitSoundComponent->SetVolumeMultiplier(0.7f);
	HitSoundComponent->bOverrideAttenuation = true;
	HitSoundComponent->AttenuationOverrides.bAttenuate = true;
	HitSoundComponent->AttenuationOverrides.bSpatialize = true;
	HitSoundComponent->AttenuationOverrides.AttenuationShape = EAttenuationShape::Sphere;
	HitSoundComponent->AttenuationOverrides.AttenuationShapeExtents = FVector(200.f, 0.f, 0.f);
	HitSoundComponent->AttenuationOverrides.FalloffDistance = 3000.0f;
	HitSoundComponent->AttenuationOverrides.DistanceAlgorithm = EAttenuationDistanceModel::Linear;

	NavInvokerComp = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));
	NavInvokerComp->SetGenerationRadii(5000.f, 6000.f);

	//★★★TraceStart와 End는 자식 클래스에서 필요한 Bone에 SetupAttachment가 필요함. Base에서는 임시로 RootComponent에 부착함.★★★
	//★★★Monster/BossMonster/Sevarog/DW_Sevarog.cpp의 생성자에서 부착 해 놓은 예시가 있음★★★
	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(RootComponent);

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(RootComponent);

	Tags.Add(TEXT("Monster"));
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f); // 회전 속도 조절

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> ShakeClass(TEXT("/Game/BluePrint/Monster/Etc/CameraShakeBase"));
	if (ShakeClass.Succeeded())
	{
		DefaultHitCameraShake = ShakeClass.Class;
	}
	
}

void ADW_MonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(DataTable))
	{
		SetStats(DataTable);
	}

	CastPlayerCharacter();
}

void ADW_MonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking)
	{
		PerformAttackTrace();
		PrevTraceStartVector = TraceStart->GetComponentLocation();
		PrevTraceEndVector = TraceEnd->GetComponentLocation();
	}
}

void ADW_MonsterBase::ResetRoot()
{
	FRotator InitialRotation = GetActorRotation();
	InitialRotation.Pitch = 0.f;
	InitialRotation.Roll = 0.f;
	SetActorRotation(InitialRotation);
}

void ADW_MonsterBase::SetMovementSpeed(int32 const NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ADW_MonsterBase::SetAccelerationSpeed(int32 const NewAccelerationSpeed)
{
	GetCharacterMovement()->MaxAcceleration = NewAccelerationSpeed;
}

EMonsterState ADW_MonsterBase::GetCurrentState() const
{
	return CurrentState;
}

void ADW_MonsterBase::SetCurrentState(EMonsterState MonsterState)
{
	CurrentState = MonsterState;
}

void ADW_MonsterBase::SetStats(UDataTable* NewDataTable)
{
	if (IsValid(NewDataTable))
	{
		FName RowName = FName(*StaticEnum<EMonsterName>()->GetNameStringByValue(static_cast<int64>(MonsterName)));

		const FString ContextString(TEXT("Monster Stat Lookup"));
		FMonsterStatsTable* StatRow = NewDataTable->FindRow<FMonsterStatsTable>(RowName, ContextString);

		if (StatRow)
		{
			// 스탯 적용
			MonsterMaxHP = StatRow->MaxHP;
			MonsterHP = StatRow->HP;
			MonsterDamage = StatRow->Damage;
			MonsterSpeed = StatRow->MoveSpeed;
			MonsterAccelSpeed = StatRow->AccelSpeed;
		}
	}

	SetMovementSpeed(MonsterSpeed);
	SetAccelerationSpeed(MonsterAccelSpeed);
}

FName ADW_MonsterBase::GetMonsterName() const
{
	return FName(*StaticEnum<EMonsterName>()->GetNameStringByValue(static_cast<int64>(MonsterName)));
}

float ADW_MonsterBase::GetMonsterMaxHP() const
{
	return MonsterMaxHP;
}

float ADW_MonsterBase::GetMonsterHP() const
{
	return MonsterHP;
}

float ADW_MonsterBase::GetMonsterDamage() const
{
	return MonsterDamage;
}

float ADW_MonsterBase::GetMonsterSpeed() const
{
	return MonsterSpeed;
}

void ADW_MonsterBase::SetMonsterDamageMultiplier(float NewMultiplier)
{
	MonsterDamageMultiplier = NewMultiplier;
}

void ADW_MonsterBase::SetMonsterSpeed(float NewSpeed)
{
	MonsterSpeed = NewSpeed;
	SetMovementSpeed(MonsterSpeed);
}

void ADW_MonsterBase::SetMonsterAccelSpeed(float NewAccelSpeed)
{
	MonsterAccelSpeed = NewAccelSpeed;
	SetAccelerationSpeed(MonsterAccelSpeed);
}

void ADW_MonsterBase::PerformAttack(int32 PatternIndex)
{
	if (IsValid(AnimMontages[PatternIndex]))
	{
		UAnimMontage* Montage = AnimMontages[PatternIndex];
		
		if (Montage && GetMesh())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(Montage);
		}
	}
}

void ADW_MonsterBase::InitialSpawn()
{
	if (IsValid(SpawnMontage))
	{
		UAnimMontage* Montage = SpawnMontage;
		
		if (Montage && GetMesh())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(Montage);
		}
	}
}

void ADW_MonsterBase::PlayParryingMontage()
{	
	if (IsValid(ParriedMontage))
	{
		UAnimMontage* Montage = ParriedMontage;
		
		if (Montage && GetMesh())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(Montage);
		}
	}
}

void ADW_MonsterBase::PlayHitMontage()
{
	bIsAttacking = false;
	bCanParried = false;
	
	if (HitMontages.Num() > 0)
	{
		int32 RandomValue = 0;
		int32 const MontageSize = HitMontages.Num();
		RandomValue = FMath::RandRange(0, MontageSize - 1);

		if (IsValid(HitMontages[RandomValue]))
		{
			UAnimMontage* Montage = HitMontages[RandomValue];
		
			if (Montage && GetMesh())
			{
				GetMesh()->GetAnimInstance()->Montage_Play(Montage);
			}
		}
	}
}

int32 ADW_MonsterBase::GetRandomMontage()
{
	if (AnimMontages.Num() > 0)
	{
		int32 const MontageSize = AnimMontages.Num();
		int32 const RandomValue = FMath::RandRange(0, MontageSize - 1);

		return RandomValue;
	}
	else
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("Montage가 없삼"));
#endif
		return 0;
	}
}

// void ADW_MonsterBase::SetRandomAttackKey(int32 PatternIndex)
// {
// 	if (AAIController* Ctr = Cast<AAIController>(GetController()))
// 	{
// 		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
// 		{
// 			BBC->SetValueAsBool(FName("RandomAttackKey"), true);
// 		}
// 	}
// }

void ADW_MonsterBase::PlayAttackSound(const int32 SoundIndex)
{
	if (AttackSoundComponent && AttackSounds[SoundIndex])
	{
		AttackSoundComponent->SetSound(AttackSounds[SoundIndex]);
		AttackSoundComponent->Play();
	}
}

void ADW_MonsterBase::PlayHitSound()
{
	if (AttackSoundComponent && HitSounds.Num() > 0)
	{
		const int32 RandomValue = FMath::RandRange(0, HitSounds.Num() - 1);
		AttackSoundComponent->SetSound(HitSounds[RandomValue]);
		AttackSoundComponent->Play();
	}
}

void ADW_MonsterBase::CanParry()
{
	bCanParried = true;
}

void ADW_MonsterBase::CantParry()
{
	bCanParried = false;
}

bool ADW_MonsterBase::GetCanParry()
{
	return bCanParried;
}

void ADW_MonsterBase::StartAttackTrace()
{
	bIsAttacking = true;
	AlreadyAttackingActors.Empty();

	PrevTraceStartVector = TraceStart->GetComponentLocation();
	PrevTraceEndVector = TraceEnd->GetComponentLocation();
	
}

void ADW_MonsterBase::EndAttackTrace()
{
	bIsAttacking = false;
	AlreadyAttackingActors.Empty();
}

void ADW_MonsterBase::PerformAttackTrace()
{
	const FVector CurrStart = TraceStart->GetComponentLocation();
	const FVector CurrEnd = TraceEnd->GetComponentLocation();

	const int NumSteps = 5;
	const float CapsuleRadius = 40.f;

	for (int i = 0; i < NumSteps; ++i)
	{
		float Alpha = static_cast<float>(i) / (NumSteps - 1);
		FVector Prev = FMath::Lerp(PrevTraceStartVector, PrevTraceEndVector, Alpha);
		FVector Curr = FMath::Lerp(CurrStart, CurrEnd, Alpha);

		FVector Segment = Curr - Prev;
		float Length = Segment.Size();
		if (Length < KINDA_SMALL_NUMBER)
		{
			continue;
		}

		FVector Direction = Segment / Length;
		FQuat Rotation = FQuat::FindBetweenNormals(FVector(0, 0, 1), Direction);

		FVector Center = (Prev + Curr) / 2.0f;
		float CapsuleHalfHeight = Length / 2.0f;

		FHitResult Hit;
		FCollisionQueryParams Params;

		if (bDrawDebugTrace)
		{
#if WITH_EDITOR
			DrawDebugCapsule(GetWorld(), Center, CapsuleHalfHeight, CapsuleRadius, Rotation, FColor::Green, false, DebugDrawTime);
#endif
		}

		if (GetWorld()->SweepSingleByChannel(Hit, Prev, Curr, Rotation, ECC_Pawn, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), Params))
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (!AlreadyAttackingActors.Contains(HitActor) && HitActor->IsA(ADW_CharacterBase::StaticClass()))
				{
					AlreadyAttackingActors.Add(HitActor);

					const float DamageAmount = MonsterDamage * MonsterDamageMultiplier;
					const FVector HitFromDirection = -GetActorForwardVector();
					const FVector HitLocation = Hit.ImpactPoint;
					const FHitResult HitResult = Hit;

					UGameplayStatics::ApplyPointDamage(
						HitActor,
						DamageAmount,
						HitFromDirection,
						HitResult,
						nullptr,
						this,
						nullptr
					);

					if (IsValid(HitImpactNS))
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(
							GetWorld(),
							HitImpactNS,
							HitLocation,
							GetActorRotation(),
							FVector(1.f)
						);
					}

					if (bCanKnockbackByAttack)
					{
						if (ADW_CharacterBase* Character = Cast<ADW_CharacterBase>(HitActor))
						Character->KnockBackCharacter();
					}
				}
			}
		}
	}
}

void ADW_MonsterBase::Parried()
{
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("Parry"));
#endif

	bIsAttacking = false;
	bCanParried = false;

	if (AAIController* Ctr = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
		{
			//상태이상은 2가지이므로 하나가 True 된다면 나머지가 false 되어야 함
			BBC->SetValueAsBool(FName("bIsStaggered"), false);
			BBC->SetValueAsBool(FName("Parried"), true);
		}
	}
}

void ADW_MonsterBase::Dead()
{
	if (bIsDead) return;
	
	bIsDead = true;
	DropItem(DropTable); 
	
	if (DropTable)
	{
		FName RowName = FName(*StaticEnum<EMonsterName>()->GetNameStringByValue(static_cast<int64>(MonsterName)));
		const FMonsterDropTable* DropData = DropTable->FindRow<FMonsterDropTable>(RowName, TEXT(""));

		if (DropData && DropData->DropItems.Num() > 0)
		{
			EItemGrade HighestGrade = EItemGrade::Normal;
			for (const FDropItemData& ItemData : DropData->DropItems)
			{
				EItemGrade CurrentGrade;
				int32 EnchantLevel;
				FString ItemRowID_FString; 
				bool bIsSuccess;
				
				UItemTranslator::ParseItemCode(ItemData.ItemCode, CurrentGrade, EnchantLevel, ItemRowID_FString, bIsSuccess);
				
				if (bIsSuccess && static_cast<int32>(CurrentGrade) > static_cast<int32>(HighestGrade))
				{
					HighestGrade = CurrentGrade;
				}
			}
			FString VFX_Path;
			switch (HighestGrade)
			{
				case EItemGrade::Normal:   VFX_Path = TEXT("NiagaraSystem'/Game/DropItem_Vfx/NE_Drop_Normal.NE_Drop_Normal'");     break;
				case EItemGrade::Rare:     VFX_Path = TEXT("NiagaraSystem'/Game/DropItem_Vfx/NE_Drop_Rare.NE_Drop_Rare'");         break;
				case EItemGrade::Unique:   VFX_Path = TEXT("NiagaraSystem'/Game/DropItem_Vfx/NE_Drop_Unique.NE_Drop_Unique'");     break;
				case EItemGrade::Legendary:VFX_Path = TEXT("NiagaraSystem'/Game/DropItem_Vfx/NE_Drop_Legendery.NE_Drop_Legendery'"); break;
				default:                   VFX_Path = TEXT("");                                                                 break;
			}

			if (!VFX_Path.IsEmpty())
			{
				UNiagaraSystem* VFX_ToSpawn = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, *VFX_Path));
				if (VFX_ToSpawn)
				{
					SpawnedVFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFX_ToSpawn, GetActorLocation());
				}
			}
		}
	}
	
	// 3. 마지막으로 사망 애니메이션을 재생하고 AI를 정지시킵니다.
	if (IsValid(DeadMontage))
	{
		UAnimMontage* Montage = DeadMontage;
		if (Montage && GetMesh())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(Montage);
			if (AAIController* AIController = Cast<AAIController>(GetController()))
			{
				if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
				{
					BTComp->StopTree(EBTStopMode::Forced);
					GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
			}
		}
	}
}


float ADW_MonsterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (bIsGuard)
	{
		if (GuardHitNS)
		{
			if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
			{
				const FPointDamageEvent& PointEvent = static_cast<const FPointDamageEvent&>(DamageEvent);
				const FVector HitLocation = PointEvent.HitInfo.ImpactPoint;

				const FVector SpawnLocation = HitLocation;
				const FRotator SpawnRotation = GetActorRotation();

				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					GuardHitNS,
					SpawnLocation,
					SpawnRotation,
					FVector(1.f),
					true,
					true
				);
			}
		}
	}
	else
	{
		if (HitNS)
		{
			if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
			{
				const FPointDamageEvent& PointEvent = static_cast<const FPointDamageEvent&>(DamageEvent);
				const FVector HitLocation = PointEvent.HitInfo.ImpactPoint;

				const FVector SpawnLocation = HitLocation;
				const FRotator SpawnRotation = GetActorRotation();

				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					HitNS,
					SpawnLocation,
					SpawnRotation,
					FVector(1.f),
					true,
					true
				);
			}
		}
	}

	if (bIsDead) return 0;

	if (bIsInvincible)
	{
		DamageAmount = 0;
	}
	
	MonsterHP = FMath::Clamp(MonsterHP - DamageAmount, 0, MonsterMaxHP);

	HitStop(0.2f);
	PlayHitSound();

	
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		const FHitResult& Hit = PointEvent->HitInfo;

		UMaterialInterface* DecalMaterial = Cast<UMaterialInterface>(
			StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/BluePrint/Monster/Etc/M_HitDecal.M_HitDecal"))
		);

		if (DecalMaterial)
		{
			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(
				DecalMaterial,
				FVector(50.f),
				GetMesh(),
				NAME_None,
				GetMesh()->GetComponentTransform().InverseTransformPosition(Hit.ImpactPoint),
				Hit.ImpactNormal.Rotation(),
				EAttachLocation::KeepRelativeOffset,
				0.0f
			);

			if (Decal)
			{
				Decal->SetFadeOut(0.5f, 1.0f, false);
			}
		}
	}
	


	if (DefaultHitCameraShake)
	{
		UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(DefaultHitCameraShake);
	}


	bIsGuard = false;

	if (MonsterHP <= 0)
	{
		Dead();

		return 0;
	}

	if (DamageAmount >= MonsterMaxHP * 0.3f)
	{
		if (AAIController* Ctr = Cast<AAIController>(GetController()))
		{
			if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
			{
				//상태이상은 2가지이므로 하나가 True 된다면 나머지가 false 되어야 함
				BBC->SetValueAsBool(FName("bIsStaggered"), true);
				BBC->SetValueAsBool(FName("Parried"), false);
			}
		}
	}
	
	return 0;
}

void ADW_MonsterBase::CastPlayerCharacter()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADW_CharacterBase::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ADW_CharacterBase* Character = Cast<ADW_CharacterBase>(Actor))
		{
			PlayerCharacter = Character;
				// if (AAIController* Ctr = Cast<AAIController>(GetController()))
				// {
				// 	if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
				// 	{
				// 		BBC->SetValueAsObject(FName("TargetActor"), Actor);
				// 	}
				// }
		}
	}
}

ADW_CharacterBase* ADW_MonsterBase::GetPlayerCharacter()
{
	return PlayerCharacter;
}

float ADW_MonsterBase::GetPlayerDistance()
{
	if (!IsValid(PlayerCharacter))
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("GetPlayerDistance: PlayerCharacter 참조 실패, -1.0f 반환"));
#endif
		return -1.0f; // 유효하지 않으면 음수 리턴
	}

	return FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());
}

bool ADW_MonsterBase::CanBeCut_Implementation(const FHitResult& Hit)
{
	return true;
}

void ADW_MonsterBase::DropItem(UDataTable* NewDataTable)
{
	if (!IsValid(NewDataTable)) return;

	FName RowName = FName(*StaticEnum<EMonsterName>()->GetNameStringByValue(static_cast<int64>(MonsterName)));
	const FString ContextString(TEXT("Monster Stat Lookup"));

	FMonsterDropTable* DropData = NewDataTable->FindRow<FMonsterDropTable>(RowName, ContextString);
	if (!DropData) return;

	for (const FDropItemData& ItemData : DropData->DropItems)
	{
		if (ItemData.DropItem && FMath::FRand() <= ItemData.DropChance)
		{
			//FVector RandOffset = FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), 0);
			FVector SpawnLocation = GetActorLocation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			AWorldItemActor* ItemActor = GetWorld()->SpawnActor<AWorldItemActor>(
				ItemData.DropItem,
				SpawnLocation,
				FRotator::ZeroRotator,
				SpawnParams
			);

			ItemActor->SetItemCode(ItemData.ItemCode);
			ItemActor->SetOwnerMonster(this);
			
			int32 ItemCount;
			if (ItemData.bUseMinDropCount)
			{
				ItemCount = FMath::RandRange(ItemData.MinDropCount, ItemData.DropCount);
			}
			else
			{
				ItemCount = ItemData.DropCount;
			}
			ItemActor->SetItemCount(ItemCount);
		}
	}
}

void ADW_MonsterBase::ResetAttakingActors()
{
	AlreadyAttackingActors.Empty();
}

void ADW_MonsterBase::HitStop(float StopTime)
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	UGameplayStatics::SetGlobalTimeDilation(World, 0.001f);
	
	World->GetTimerManager().SetTimer(HitStopTimerHandle, [World]()
	{
		UGameplayStatics::SetGlobalTimeDilation(World, 1.0f);
	}, 0.001f * StopTime, false);
}
void ADW_MonsterBase::DestroySpawnedVFX()
{
	if (SpawnedVFX && SpawnedVFX->IsValidLowLevel())
	{
		SpawnedVFX->Deactivate(); 
		
		SpawnedVFX->DestroyComponent(); 
		
		SpawnedVFX = nullptr; 
	}
}
void ADW_MonsterBase::IncreaseMastery(UDataTable* NewDataTable)
{
}
