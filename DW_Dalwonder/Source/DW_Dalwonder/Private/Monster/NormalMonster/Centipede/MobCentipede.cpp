// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Centipede/MobCentipede.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/DW_CharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AMobCentipede::AMobCentipede()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TraceStart->SetupAttachment(GetMesh(), TEXT("hand_r"));
	TraceEnd->SetupAttachment(GetMesh(), TEXT("hand_r"));
}

void AMobCentipede::BeginPlay()
{
	Super::BeginPlay();

}

void AMobCentipede::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bCanRotation)
	{
		if (GetPlayerCharacter())
		{
			if (AAIController* AIC = Cast<AAIController>(GetController()))
			{
				if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
				{
					if (BB->GetValueAsEnum(FName("CurrentState")) == 2)
					{
						FRotator CurrentRot = GetActorRotation();
						FVector TargetLocation = GetPlayerCharacter()->GetActorLocation();
						FVector MyLocation = GetActorLocation();

						FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
						TargetRot.Pitch = 0.f;
						TargetRot.Roll = 0.f;

						FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);

						SetActorRotation(NewRot);
					}
				}
			}
		}
	}
}

void AMobCentipede::CanRotationSetting(bool Value)
{
	bCanRotation = Value;
}
