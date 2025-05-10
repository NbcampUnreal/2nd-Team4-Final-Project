// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/DW_MonsterBase.h"

#include "AIController.h"
#include "DW_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DataTable.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


// Sets default values
ADW_MonsterBase::ADW_MonsterBase(): CurrentState(EMonsterState::Idle), DataTable(nullptr),
                                    AttackSoundComponent(nullptr), HitSoundComponent(nullptr), bIsAttacking(false), bCanParried(false),
                                    PlayerCharacter(nullptr), MonsterHP(0), MonsterDamage(0), MonsterSpeed(0)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AttackSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackSound"));
	AttackSoundComponent->SetupAttachment(RootComponent);
	AttackSoundComponent->bAutoActivate = false;

	HitSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSound"));
	HitSoundComponent->SetupAttachment(RootComponent);
	HitSoundComponent->bAutoActivate = false;

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

// Called when the game starts or when spawned
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
	//데이터 테이블 완성 후, MonsterName, MonsterHP, MonsterDamage, MonsterSpeed 초기화 예정
}

FName ADW_MonsterBase::GetMonsterName() const
{
	return MonsterName;
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
		UE_LOG(LogTemp, Error, TEXT("Montage가 없삼"));
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

	const int NumSteps = 3;
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
			DrawDebugLine(GetWorld(), Prev, Curr, FColor::Red, false, DebugDrawTime, 0, 2.f);
			DrawDebugSphere(GetWorld(), Curr, 5.f, 12, FColor::Yellow, false, DebugDrawTime);
		}

		if (GetWorld()->LineTraceSingleByChannel(Hit, Prev, Curr, ECC_Pawn, Params))
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (!AlreadyAttackingActors.Contains(HitActor))
				{
					AlreadyAttackingActors.Add(HitActor);

					// 데미지 처리
					UGameplayStatics::ApplyDamage(HitActor, 20.f, nullptr, this, nullptr);

					UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
				}
			}
		}
	}
}

void ADW_MonsterBase::Parried()
{
	UE_LOG(LogTemp, Warning, TEXT("Parry"));

	bIsAttacking = false;
	bCanParried = false;

	if (AAIController* Ctr = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
		{
			BBC->SetValueAsBool(FName("Parried"), true);
		}
	}
}

//데미지 받을 때의 함수, 구현 필요
float ADW_MonsterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	return 0;
}

//데미지 가할 때의 함수, 구현 필요
float ADW_MonsterBase::ApplyDamage(AActor* DamagedActor, float BaseDamage, AController* EventInstigator,
	AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
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
				if (AAIController* Ctr = Cast<AAIController>(GetController()))
				{
					if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
					{
						BBC->SetValueAsObject(FName("TargetActor"), Actor);
					}
				}
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
		return -1.0f; // 유효하지 않으면 음수 리턴
	}

	return FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());
}

