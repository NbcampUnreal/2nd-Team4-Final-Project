// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Arachnid/BossArachnid.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/DW_CharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

ABossArachnid::ABossArachnid()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("Front1"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("Front2"));
}

void ABossArachnid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanRotate)
	{
		if (GetPlayerCharacter())
		{
			FRotator CurrentRot = GetActorRotation();
			FVector TargetLocation = GetPlayerCharacter()->GetActorLocation();
			FVector MyLocation = GetActorLocation();

			FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);

			FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);

			SetActorRotation(NewRot);
		}
	}
}

float ABossArachnid::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(bIsPhaseTwo == false && MonsterHP <= MonsterMaxHP / 2)
	{
		if (AAIController* Ctr = Cast<AAIController>(GetController()))
		{
			if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
			{
				bIsPhaseTwo = true;

				BBC->SetValueAsBool(FName("bIsPhase_Two"), true);
				BBC->SetValueAsBool(FName("bCanUndead"), true);
			}
		}
	}

	if (bIsPhaseTwo == true && bCanBurrow == false && MonsterHP <= MonsterMaxHP / 10)
	{
		if (AAIController* Ctr = Cast<AAIController>(GetController()))
		{
			if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
			{
				bCanBurrow = true;

				BBC->SetValueAsBool(FName("bCanUndead"), true);
			}
		}
	}

	return 0.0f;
}

void ABossArachnid::RotationEnable(bool Value)
{
	bCanRotate = Value;
}

void ABossArachnid::ChangingAttackTrace(int32 Value)
{
	switch (Value)
	{
	case 0:
		TraceStart->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Front1"));
		TraceEnd->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Front2"));
		break;

	case 1:
		TraceStart->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Right"));
		TraceEnd->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Right"));
		break;

	case 2:
		TraceStart->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Left"));
		TraceEnd->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Left"));
		break;
	/*case 0:
		TraceStart->SetupAttachment(GetMesh(), TEXT("Front1"));
		TraceEnd->SetupAttachment(GetMesh(), TEXT("Front2"));
		break;

	case 1:
		TraceStart->SetupAttachment(GetMesh(), TEXT("Right"));
		TraceEnd->SetupAttachment(GetMesh(), TEXT("Right"));
		break;

	case 2:
		TraceStart->SetupAttachment(GetMesh(), TEXT("Left"));
		TraceEnd->SetupAttachment(GetMesh(), TEXT("Left"));
		break;*/

	default:
		break;
	}
}
