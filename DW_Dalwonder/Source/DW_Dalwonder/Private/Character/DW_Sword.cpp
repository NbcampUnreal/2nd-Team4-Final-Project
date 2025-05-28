#include "Character/DW_Sword.h"
#include "Monster/DW_MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"

ADW_Sword::ADW_Sword()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Root);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	SwordTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStartPoint"));
	SwordTraceStartPoint->SetupAttachment(Root);
	SwordTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEndPoint"));
	SwordTraceEndPoint->SetupAttachment(Root);
}

void ADW_Sword::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(SwordTraceStartPoint) && IsValid(SwordTraceEndPoint))
	{
		// 시작점과 끝점 사이 거리 측정
		const float TraceLength = FVector::Dist(
			SwordTraceStartPoint->GetComponentLocation(),
			SwordTraceEndPoint->GetComponentLocation());

		// 길이 기준으로 Capsule 크기 자동 설정
		CapsuleTraceHalfHeight = TraceLength * 0.5f;
		CapsuleTraceRadius = FMath::Clamp(TraceLength * 0.1f, 5.f, 20.f); // 필요 시 조정

		UE_LOG(LogTemp, Log, TEXT("Sword CapsuleTrace 설정: HalfHeight=%.1f, Radius=%.1f"),
			CapsuleTraceHalfHeight, CapsuleTraceRadius);
	}
}
