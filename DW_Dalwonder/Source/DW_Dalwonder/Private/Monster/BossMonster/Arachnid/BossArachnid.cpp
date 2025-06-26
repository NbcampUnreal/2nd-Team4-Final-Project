// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Arachnid/BossArachnid.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/DW_CharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

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

	//if (bCanRotate)
	//{
	//	if (GetPlayerCharacter())
	//	{
	//		/*FRotator CurrentRot = GetControlRotation();
	//		FVector TargetLocation = GetPlayerCharacter()->GetActorLocation();
	//		FVector MyLocation = GetActorLocation();

	//		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);

	//		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);

	//		if (AController* MyController = GetController())
	//		{
	//			MyController->SetControlRotation(NewRot);
	//		}*/

	//		/*FRotator CurrentRot = GetActorRotation();
	//		FVector TargetLocation = GetPlayerCharacter()->GetActorLocation();
	//		FVector MyLocation = GetActorLocation();

	//		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);

	//		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);

	//		SetActorRotation(NewRot);*/
	//	}
	//}

	if (bShouldTurn)
	{
		/*UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance) return;

		float RotOffset = AnimInstance->GetCurveValue(FName("Rotate"));*/

		FVector TargetLocation = GetPlayerCharacter()->GetActorLocation();
		FVector MyLocation = GetActorLocation();

		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		FRotator Rotate = GetActorRotation();

		RotationDot = TargetRot.Yaw - Rotate.Yaw;

		FRotator NewRot = FMath::RInterpTo(Rotate, TargetRot, DeltaTime, TurnInPlaceSpeed);

		//Rotate.Yaw = CurrentYaw + RotOffset;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Yaw : %f"), RotOffset));
		SetActorRotation(NewRot);
	}

	if (bIsJumping)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance) return;

		float JumpZ = AnimInstance->GetCurveValue(FName("Arachnid_Jump_Z"));
		float JumpX = AnimInstance->GetCurveValue(FName("Arachnid_Jump_X"));

		FVector JumpingLocation = GetActorLocation();
		JumpingLocation.Z = CurrentVector.Z + (JumpZ * JumpZMultiplier);
		JumpingLocation.X = CurrentVector.X + (JumpX * JumpXMultiplier) * -GetActorForwardVector().X;
		SetActorLocation(JumpingLocation);
	}

	if (bShouldRush)
	{

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance) return;

		float Dash = AnimInstance->GetCurveValue(FName("Arachnid_Rushhh"));
		//float Dash = AnimInstance->GetCurveValue(FName("TestCurve"));

		/*FVector RushDirection = GetActorForwardVector();
		FVector RushOffset = RushDirection * Dash;

		FVector RushLocation = GetActorLocation();*/

		//RushLocation.X = CurrentVector_Rush.X + Dash * GetActorForwardVector().X;
		//RushLocation.Y = CurrentVector_Rush.Y + Dash * GetActorForwardVector().Y;
		//SetActorLocation(CurrentVector_Rush + RushOffset);

		
			SetActorLocation(CurrentVector_Rush + GetActorForwardVector() * Dash);
		
			if (Dash > 1000.f)
			{
				bShouldRush = false;
				return;
			}



		//UE_LOG(LogTemp, Warning, TEXT("Dash : (%f)"), Dash);

		//UE_LOG(LogTemp, Warning, TEXT("RushLocation : (%f, %f, %f)"), RushLocation.X, RushLocation.Y, RushLocation.Z);
	}
}

float ABossArachnid::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsUndead)
	{
		DamageAmount /= 3.f;
	}

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
	//bCanRotate = Value;

	if (Value)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
	else
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
}

void ABossArachnid::DoTrunInPlace(bool Value)
{
	bShouldTurn = Value;
	CurrentYaw = GetActorRotation().Yaw;
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

void ABossArachnid::ArachnidJumpOn()
{
	CurrentVector = GetActorLocation();
	bIsJumping = true;
}

void ABossArachnid::ArachnidJumpOff()
{
	bIsJumping = false;

}

void ABossArachnid::ArachnidRushOn()
{
	CurrentVector_Rush = GetActorLocation();
	bShouldRush = true;

}

void ABossArachnid::ArachnidRushOff()
{
	bShouldRush = false;
}

void ABossArachnid::UndeadOn()
{
	bIsUndead = true;

	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(UndeadTimerHandle, this, &ABossArachnid::Undead, 3.f, false);
	}
}

void ABossArachnid::UndeadOff()
{
	bIsUndead = false;
}

void ABossArachnid::Dead()
{
	Super::Dead();

	bShouldTurn = false;
	bCanRotate = false;
	bIsJumping = false;
	bIsUndead = false
}

void ABossArachnid::Undead()
{
	if (!bIsUndead) return;

	MonsterHP += MonsterMaxHP / 20.f;

	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(UndeadTimerHandle, this, &ABossArachnid::Undead, 1.f, false);
	}
}
