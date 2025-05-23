#include "Monster/NormalMonster/DW_NormalMonsterBase.h"

#include "AIController.h"
#include "Character/DW_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ADW_NormalMonsterBase::ADW_NormalMonsterBase(): bIsAlerted(false), bIsFirstResponder(true), MonsterAlertDistance(0),
                                                AlertMontage(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	Tags.Add(TEXT("NormalMonster"));
}

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

				if (AAIController* AICon = Cast<AAIController>(Other->GetController()))
				{
					if (UBlackboardComponent* BBC = AICon->GetBlackboardComponent())
					{
						BBC->SetValueAsObject(FName("TargetActor"), PlayerCharacter);
						BBC->SetValueAsBool(FName("bIsPlayerFound"), true);
					}
				}
				
				Execute_FoundPlayer(Other);
			}
		}
	}
}

void ADW_NormalMonsterBase::FoundPlayer_Implementation()
{
	// 블랙보드 설정
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = AICon->GetBlackboardComponent())
		{
			BBC->SetValueAsObject(FName("TargetActor"), PlayerCharacter);
			BBC->SetValueAsBool(FName("bIsPlayerFound"), true);
		}
	}

	if (bIsAlerted)
	{
		return;
	}
	
	bIsAlerted = true;
	
	// 최초 감지자만 몽타주 재생
	if (bIsFirstResponder)
	{
		PlayAlertMontage();
		
	}
}

//void ADW_NormalMonsterBase::LosePlayer_Implementation()
//{
//	if (AAIController* AICon = Cast<AAIController>(GetController()))
//	{
//		if (UBlackboardComponent* BBC = AICon->GetBlackboardComponent())
//		{
//			BBC->SetValueAsObject(FName("TargetActor"), nullptr);
//			BBC->SetValueAsBool(FName("bIsPlayerFound"), false);
//		}
//	}
//}

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

void ADW_NormalMonsterBase::ResetAlert()
{
	bIsAlerted = false;
	bIsFirstResponder = true;
}

void ADW_NormalMonsterBase::SaveRoot()
{
	//InitialRotation = GetActorRotation();
}

void ADW_NormalMonsterBase::ResetRoot()
{
	FRotator InitialRotation = GetActorRotation();
	InitialRotation.Pitch = 0.f;
	InitialRotation.Roll = 0.f;
	SetActorRotation(InitialRotation);
}
