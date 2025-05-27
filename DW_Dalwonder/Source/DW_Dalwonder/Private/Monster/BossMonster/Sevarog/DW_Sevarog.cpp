// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Sevarog/DW_Sevarog.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/MonsterStatsTable.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"


// Sets default values
ADW_Sevarog::ADW_Sevarog()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//부착 후 에디터에서 위치 세부 조정 필요
	TraceStart->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	
	Hammer = CreateDefaultSubobject<USceneComponent>("Hammer");
	Hammer->SetupAttachment(GetMesh(), TEXT("weapon_r"));
}

float ADW_Sevarog::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentPhase == 1 && MonsterHP * 2 <= MonsterMaxHP)
	{
		SetCurrentPhase(2);
	}
	
	return 0;
}

void ADW_Sevarog::AirAttack()
{
	FVector HammerLocation = Hammer->GetComponentLocation();
	float Radius = 200.0f;

	TArray<FOverlapResult> OverlapResults; // 이게 있어야 함
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		HammerLocation,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);

	if (bHit)
	{
		for (const FOverlapResult& Result : OverlapResults) // 이 Result는 위 TArray에서 나옴
		{
			AActor* HitActor = Result.GetActor(); // 여기서 오류가 없어야 정상
			if (HitActor && HitActor != this)
			{
				UGameplayStatics::ApplyDamage(HitActor, 30.0f, GetController(), this, UDamageType::StaticClass());
			}
		}
	}

	DrawDebugSphere(GetWorld(), HammerLocation, Radius, 16, FColor::Red, false, 1.0f);
}
