// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/Elephant/DW_Elephant.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ADW_Elephant::ADW_Elephant()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
		
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

float ADW_Elephant::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AAIController* Ctr = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
		{
			BBC->SetValueAsBool(FName("bHasDamaged"), true);
		}
	}

	return 0;
}