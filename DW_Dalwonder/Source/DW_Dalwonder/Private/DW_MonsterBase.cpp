// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_MonsterBase.h"

#include "AIController.h"
#include "DW_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DataTable.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


// Sets default values
ADW_MonsterBase::ADW_MonsterBase(): CurrentState(EMonsterState::Idle), DataTable(nullptr),
                                    AttackSoundComponent(nullptr), HitSoundComponent(nullptr),
                                    HitSound(nullptr),
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

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1800.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1200.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
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

void ADW_MonsterBase::SetRandomAttackKey(int32 PatternIndex)
{
	if (AAIController* Ctr = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
		{
			BBC->SetValueAsBool(FName("RandomAttackKey"), true);
		}
	}
}

void ADW_MonsterBase::PlayAttackSound(int32 SoundIndex)
{
	if (AttackSoundComponent && AttackSounds[SoundIndex])
	{
		AttackSoundComponent->SetSound(AttackSounds[SoundIndex]);
		AttackSoundComponent->Play();
	}
}

void ADW_MonsterBase::PlayHitSound()
{
	if (AttackSoundComponent && HitSound)
	{
		AttackSoundComponent->SetSound(HitSound);
		AttackSoundComponent->Play();
	}
}

//데미지 받을 때의 함수, 구현 필요
float ADW_MonsterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	return 0;
}

//데미지 가할 떄의 함수, 구현 필요
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

