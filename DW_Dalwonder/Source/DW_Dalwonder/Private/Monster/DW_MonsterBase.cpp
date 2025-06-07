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
#include "Character/DW_PlayerController.h"
#include "Monster/BossMonster/DW_BossMonsterBase.h"
#include "UI/Widget/BossHUDWidget.h"
#include "DW_GmBase.h"
#include "Engine/DamageEvents.h"

ADW_MonsterBase::ADW_MonsterBase(): CurrentState(EMonsterState::Idle), DataTable(nullptr),
                                    AttackSoundComponent(nullptr), HitSoundComponent(nullptr), bIsAttacking(false), bCanParried(false),
                                    PlayerCharacter(nullptr), MonsterMaxHP(0),MonsterHP(0), MonsterDamage(0),
                                    MonsterSpeed(100), MonsterAccelSpeed(100), MonsterDamageMultiplier(1.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	AttackSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackSound"));
	AttackSoundComponent->SetupAttachment(RootComponent);
	AttackSoundComponent->bAutoActivate = false;

	HitSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSound"));
	HitSoundComponent->SetupAttachment(RootComponent);
	HitSoundComponent->bAutoActivate = false;

	NavInvokerComp = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));
	NavInvokerComp->SetGenerationRadii(5000.f, 6000.f);

	//★★★TraceStart와 End는 자식 클래스에서 필요한 Bone에 SetupAttachment가 필요함. Base에서는 임시로 RootComponent에 부착함.★★★
	//★★★Monster/BossMonster/Sevarog/DW_Sevarog.cpp의 생성자에서 부착 해 놓은 예시가 있음★★★
	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(RootComponent);

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(RootComponent);

	Tags.Add(TEXT("Monster"));
	
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 30.f, 0.f); // 회전 속도 조절
	
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
		FMonsterStatsTable* StatRow = DataTable->FindRow<FMonsterStatsTable>(RowName, ContextString);

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
	return FName("");
	//이 함수는 더미 함수임.
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
	int32 RandomValue = 0;

	bIsAttacking = false;
	bCanParried = false;
	
	if (HitMontages.Num() > 0)
	{
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

void ADW_MonsterBase::PlayAttackSound(int32 SoundIndex)
{
	if (AttackSoundComponent && AttackSounds[SoundIndex])
	{
		AttackSoundComponent->SetSound(AttackSounds[SoundIndex]);
		AttackSoundComponent->Play();
	}
}

void ADW_MonsterBase::PlayHitSound(int32 SoundIndex)
{
	if (AttackSoundComponent && HitSounds[SoundIndex])
	{
		AttackSoundComponent->SetSound(HitSounds[SoundIndex]);
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
	for (int i = 0; i < NumSteps; ++i)
	{
		float Alpha = static_cast<float>(i) / (NumSteps - 1);
		FVector Prev = FMath::Lerp(PrevTraceStartVector, PrevTraceEndVector, Alpha);
		FVector Curr = FMath::Lerp(CurrStart, CurrEnd, Alpha);

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (bDrawDebugTrace)
		{
#if WITH_EDITOR
			DrawDebugLine(GetWorld(), Prev, Curr, FColor::Red, false, DebugDrawTime, 0, 2.f);
			DrawDebugSphere(GetWorld(), Curr, 5.f, 12, FColor::Yellow, false, DebugDrawTime);
#endif
		}

		if (GetWorld()->LineTraceSingleByChannel(Hit, Prev, Curr, ECC_Pawn, Params))
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (!AlreadyAttackingActors.Contains(HitActor) && !HitActor->IsA(ADW_MonsterBase::StaticClass()))
				{
					AlreadyAttackingActors.Add(HitActor);

					// 데미지 처리
					UGameplayStatics::ApplyDamage(HitActor, MonsterDamage * MonsterDamageMultiplier, nullptr, this, nullptr);

#if WITH_EDITOR
					UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
#endif
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
	if (IsA(ADW_BossMonsterBase::StaticClass()))
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			if (ADW_PlayerController* DWPC = Cast<ADW_PlayerController>(PC))
			{
				DWPC->HideBossHUD();
			}
		}
		if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
		{
			GM->ShowResultUI("ENEMY FELLED!");
		};
	}
}


float ADW_MonsterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsA(ADW_BossMonsterBase::StaticClass())) // 보스일 때만 실행
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			if (ADW_PlayerController* DWPC = Cast<ADW_PlayerController>(PC))
			{
				if (IsValid(DWPC->CachedBossHUD))
				{
					DWPC->CachedBossHUD->UpdateHP(MonsterHP);
				}
			}
		}
	}

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

	if (bIsInvincible)
	{
		DamageAmount = 0;
	}
	
	MonsterHP = FMath::Clamp(MonsterHP - DamageAmount, 0, MonsterMaxHP);

	if (MonsterHP <= 0)
	{
		Dead();
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