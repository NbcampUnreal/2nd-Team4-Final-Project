// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Skeleton/DW_SkeletonBoss.h"


// Sets default values
ADW_SkeletonBoss::ADW_SkeletonBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	LeftHandWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandWeapon"));
	LeftHandWeapon->SetupAttachment(GetMesh(), TEXT("hand_l"));

	RightHandWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandWeapon"));
	RightHandWeapon->SetupAttachment(GetMesh(), TEXT("hand_r"));

	TraceStart->SetupAttachment(GetMesh(), TEXT("hand_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("hand_r"));
}

// Called when the game starts or when spawned
void ADW_SkeletonBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADW_SkeletonBoss::ChangeFormType()
{
	ESkeletonFormType NewFormType = GetRandomFormType();

	// 기존 값과 다른 값만 반환
	while (NewFormType == CurrentFormType)
	{
		NewFormType = GetRandomFormType();
	}
	
	CurrentFormType = NewFormType;
	
	// 현재 타입을 인덱스로 변환
	const int32 Index = static_cast<int32>(CurrentFormType);

	// 스켈레탈 구토 변환
	if (SkeletonMeshs.IsValidIndex(Index))
	{
		GetMesh()->SetSkeletalMesh(SkeletonMeshs[Index]);
	}

	// ABP 변환
	if (SkeletonABP.IsValidIndex(Index))
	{
		GetMesh()->SetAnimInstanceClass(SkeletonABP[Index]);
	}

	// 몽타주 배열 변환
	switch (CurrentFormType)
	{
	case ESkeletonFormType::Warrior:
		CurrentMontages = WarriorMontages;
		break;
	case ESkeletonFormType::Bandit:
		CurrentMontages = BanditMontages;
		break;
	case ESkeletonFormType::Archer:
		CurrentMontages = ArcherMontages;
		break;
	case ESkeletonFormType::Mage:
		CurrentMontages = MageMontages;
		break;

	default:
		CurrentMontages = WarriorMontages;
		#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("CurrentFormType 부적절한 값"));
		#endif
		break;
	}

	// 무기 변환까지 추후 추가예정
	// 블랙보드 값까지 변환예정
	// 변환 시의 나이아가라 추가예정(사운드 포함)
}

ESkeletonFormType ADW_SkeletonBoss::GetRandomFormType()
{
	constexpr int32 MaxEnumValue = static_cast<int32>(ESkeletonFormType::Max);
	int32 RandomIndex = FMath::RandRange(0, MaxEnumValue - 1);

	return static_cast<ESkeletonFormType>(RandomIndex);
}
