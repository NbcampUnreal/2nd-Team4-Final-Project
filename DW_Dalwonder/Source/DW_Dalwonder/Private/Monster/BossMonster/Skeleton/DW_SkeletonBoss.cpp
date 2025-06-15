// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Skeleton/DW_SkeletonBoss.h"


// Sets default values
ADW_SkeletonBoss::ADW_SkeletonBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	LeftHandWeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandWeapon"));
	LeftHandWeaponComp->SetupAttachment(GetMesh(), TEXT("hand_l"));

	RightHandWeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandWeapon"));
	RightHandWeaponComp->SetupAttachment(GetMesh(), TEXT("hand_r"));

	TraceStart->SetupAttachment(GetMesh(), TEXT("hand_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("hand_r"));
}

// Called when the game starts or when spawned
void ADW_SkeletonBoss::BeginPlay()
{
	Super::BeginPlay();

	ChangeFormType(CurrentFormType);
}

void ADW_SkeletonBoss::ChangeFormTypeByRandom()
{
	ESkeletonFormType NewFormType;
	
	do
	{
		constexpr int32 MaxEnumValue = static_cast<int32>(ESkeletonFormType::Max);
		int32 RandomIndex = FMath::RandRange(0, MaxEnumValue - 1);

		NewFormType = static_cast<ESkeletonFormType>(RandomIndex);
	}
	while (NewFormType == CurrentFormType);
	
	CurrentFormType = NewFormType;
	ChangeFormType(CurrentFormType);
}

void ADW_SkeletonBoss::ChangeFormType(const ESkeletonFormType FormType)
{

	CurrentFormType = FormType;
	
	if (const FFormData* FormData = FormDataMap.Find(CurrentFormType))
	{
		// 구토
		if (FormData->SkeletalMesh)
		{
			GetMesh()->SetSkeletalMesh(FormData->SkeletalMesh);
		}

		// ABP
		if (FormData->AnimBP)
		{
			GetMesh()->SetAnimInstanceClass(FormData->AnimBP);
		}

		// 무기(왼손)
		if (LeftHandWeaponComp && FormData->LeftHandWeapon)
		{
			LeftHandWeaponComp->SetSkeletalMesh(FormData->LeftHandWeapon);
			LeftHandWeaponComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_l"));
		}

		// 무기(오른손)
		if (RightHandWeaponComp && FormData->RightHandWeapon)
		{
			RightHandWeaponComp->SetSkeletalMesh(FormData->RightHandWeapon);
			RightHandWeaponComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r"));
		}

		// 몽타주
		AnimMontages = FormData->Montages;
	}
	// TODO : 블랙보드 값까지 변환예정
	// TODO : 변환 시의 나이아가라 추가예정(사운드 포함)
}
