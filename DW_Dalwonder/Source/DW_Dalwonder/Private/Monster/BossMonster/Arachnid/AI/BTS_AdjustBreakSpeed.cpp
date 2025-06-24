// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/Arachnid/AI/BTS_AdjustBreakSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"      
#include "BehaviorTree/BehaviorTreeComponent.h"   
#include "BehaviorTree/BTService.h"           
#include "AIController.h"                       
#include "GameFramework/Character.h"        
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/GameplayStatics.h"   
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"   

UBTS_AdjustBreakSpeed::UBTS_AdjustBreakSpeed()
{
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTS_AdjustBreakSpeed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn) return;

	ACharacter* Character = Cast<ACharacter>(ControlledPawn);
	if (!Character) return;

	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
	if (!Movement) return;

	if(bIsActor)
	{
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (!TargetActor) return;
		TargetLocation = TargetActor->GetActorLocation();
	}
	else
	{
		FVector TargetLoc = OwnerComp.GetBlackboardComponent()->GetValueAsVector("TargetActor");
		TargetLocation = TargetLoc;
	}

	float Distance = FVector::Dist(Character->GetActorLocation(), TargetLocation);

	if (Distance < SlowDistance)
	{
		float CurrentSpeed = Movement->MaxWalkSpeed;
		float NewSpeed = FMath::FInterpTo(CurrentSpeed, 0.f, DeltaSeconds, BreakTime);

		Movement->MaxWalkSpeed = NewSpeed;
	}
}
