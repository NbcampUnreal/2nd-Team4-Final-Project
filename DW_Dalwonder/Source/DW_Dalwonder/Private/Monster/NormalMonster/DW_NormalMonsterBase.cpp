// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/DW_NormalMonsterBase.h"

#include "AIController.h"
#include "DW_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ADW_NormalMonsterBase::ADW_NormalMonsterBase(): bIsAlerted(false), bIsFirstResponder(true), MonsterAlertDistance(0),
                                                AlertMontage(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Tags.Add(TEXT("NormalMonster"));
}

// Called when the game starts or when spawned
void ADW_NormalMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADW_NormalMonsterBase::AlertNearbyMonsters_Implementation(const int32 AlertDistance)
{
	TArray<AActor*> OutActors;

	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		AlertDistance,
		{ UEngineTypes::ConvertToObjectType(ECC_Pawn) },
		nullptr,
		{ this },
		OutActors
	);

	for (AActor* Actor : OutActors)
	{
		if (Actor->ActorHasTag("NormalMonster") && Actor->Implements<UDW_NormalMonsterBaseInterface>())
		{
			if (ADW_NormalMonsterBase* Other = Cast<ADW_NormalMonsterBase>(Actor))
			{
				Other->bIsFirstResponder = false; // 경보 받은 애는 false
				Other->PlayerCharacter = PlayerCharacter;
				Execute_FoundPlayer(Other);
			}
		}
	}
}

void ADW_NormalMonsterBase::FoundPlayer_Implementation()
{
	if (bIsAlerted)
	{
		return;
	}
	
	SetAlerted(true);

	// 블랙보드 설정
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = AICon->GetBlackboardComponent())
		{
			BBC->SetValueAsObject(FName("TargetActor"), PlayerCharacter);
			BBC->SetValueAsBool(FName("bIsPlayerFound"), true);
		}
	}

	// 최초 감지자만 몽타주 재생
	if (bIsFirstResponder)
	{
		PlayAlertMontage();
		AlertNearbyMonsters(MonsterAlertDistance);
	}
}

void ADW_NormalMonsterBase::SetAlerted_Implementation(const bool AlertValue)
{
	bIsAlerted = AlertValue;
}

void ADW_NormalMonsterBase::PlayAlertMontage()
{
	if (IsValid(AlertMontage))
	{
		UAnimMontage* Montage = AlertMontage;
		
		if (Montage && GetMesh())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(Montage);
		}
	}
}