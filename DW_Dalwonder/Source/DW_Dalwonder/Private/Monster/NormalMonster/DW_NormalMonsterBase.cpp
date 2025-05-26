#include "Monster/NormalMonster/DW_NormalMonsterBase.h"

#include "AIController.h"
#include "Character/DW_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"

ADW_NormalMonsterBase::ADW_NormalMonsterBase(): bIsAlerted(false), bIsFirstResponder(true), MonsterAlertDistance(0),
                                                AlertMontage(nullptr), AlertDelay(1.f)
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(TEXT("NormalMonster"));
}

void ADW_NormalMonsterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ADW_NormalMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldRotateToPlayer && PlayerCharacter)
	{
		FRotator CurrentRotation = GetActorRotation();

		FVector DirectionToPlayer = PlayerCharacter->GetActorLocation() - GetActorLocation();
		DirectionToPlayer.Z = 0;

		if (!DirectionToPlayer.IsNearlyZero())
		{
			FRotator TargetRotation = DirectionToPlayer.Rotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 1.5f);

			SetActorRotation(NewRotation);

			float YawDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw));

			if (YawDifference < 5.f)
			{
				bShouldRotateToPlayer = false; // 회전 완료
			}
		}
	}
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

						Other->RotateToPlayer();
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

			if (AAIController* AICon = Cast<AAIController>(GetController()))
			{
				if (UBlackboardComponent* BBC = AICon->GetBlackboardComponent())
				{
					BBC->SetValueAsBool(FName("bCanBehavior"), false);

					GetWorldTimerManager().SetTimer(AlertDelayTimer, this, &ADW_NormalMonsterBase::BehaviorOn, AlertDelay, false);
				}
			}
		}
	}
}

void ADW_NormalMonsterBase::Dead()
{
	Super::Dead();

	FTimerHandle DeadLogicTime;
	GetWorldTimerManager().SetTimer(DeadLogicTime, this, &ADW_NormalMonsterBase::DeadLogic, DeadMontageTime, false);
}

void ADW_NormalMonsterBase::DeadLogic()
{
	SetActorTickEnabled(false);
	GetCharacterMovement()->DisableMovement();

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetSimulatePhysics(true);

	FTimerHandle DestroyDelayTimer;
	GetWorldTimerManager().SetTimer(DestroyDelayTimer, this, &ADW_NormalMonsterBase::DestroyMonster, DestroyTime, false);

}

void ADW_NormalMonsterBase::DestroyMonster()
{
	Destroy();
}

float ADW_NormalMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GetWorld())
	{
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),
			this,                                   
			DamageCauser,                                  
			DamageAmount,
			DamageCauser ? DamageCauser->GetActorLocation() : FVector::ZeroVector,
			GetActorLocation()
		);
	}

	bIsAlerted = true;

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return 0.0f;
}

void ADW_NormalMonsterBase::ResetAlert()
{
	bIsAlerted = false;
	bIsFirstResponder = true;
}

void ADW_NormalMonsterBase::ResetRoot()
{
	FRotator InitialRotation = GetActorRotation();
	InitialRotation.Pitch = 0.f;
	InitialRotation.Roll = 0.f;
	SetActorRotation(InitialRotation);
}

void ADW_NormalMonsterBase::BehaviorOn()
{
	GetWorldTimerManager().ClearTimer(AlertDelayTimer);

	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = AICon->GetBlackboardComponent())
		{
			BBC->SetValueAsBool(FName("bCanBehavior"), true);
		}
	}
}

void ADW_NormalMonsterBase::RotateToPlayer()
{
	bShouldRotateToPlayer = true;
}
