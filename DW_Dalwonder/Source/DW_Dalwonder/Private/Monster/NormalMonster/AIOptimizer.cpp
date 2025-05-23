// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster/AIOptimizer.h"
#include "BrainComponent.h"
#include "Monster/NormalMonster/SpawnType.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"


// Sets default values for this component's properties
UAIOptimizer::UAIOptimizer()
{
	PrimaryComponentTick.bCanEverTick = true;

	LayerShort = 1000.f;
	LayerMiddle = 8000.f;
	LayerLong = 9000.f;
	LayerMax = 10000.f;
}

void UAIOptimizer::BeginPlay()
{
	Super::BeginPlay();

	OptimizerChecker();
}

void UAIOptimizer::SetCharacterMovementEnabled(ACharacter* Character, bool bEnable)
{
	if (bEnable)
	{
		Character->GetCharacterMovement()->Activate();
		Character->GetCharacterMovement()->SetComponentTickEnabled(true);
	}
	else
	{
		Character->GetCharacterMovement()->Deactivate();
		Character->GetCharacterMovement()->SetComponentTickEnabled(false);
	}
}

void UAIOptimizer::SetAILogicEnabled(ACharacter* Actor, AAIController* AIC, bool bEnable)
{
	if (UBrainComponent* BrainComp = AIC->GetBrainComponent())
	{
		if (bEnable)
		{
			if (!BrainComp->IsRunning())
			{
				BrainComp->RestartLogic();
			}
		}
		else
		{
			if (BrainComp->IsRunning())
			{
				BrainComp->StopLogic(TEXT("Custom Optimizer"));
			}
		}
	}
}

void UAIOptimizer::OptimizerSetting(int32 OptimizerEnable)
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (AAIController* AIC = Cast<AAIController>(Character->GetController()))
		{

			// AI BRAIN ( BEHAVIOR TREE )
			if ((OptimizerEnable) & (1 << static_cast<uint8>(EAIOptimizerFlags::AIBrain)))
			{
				SetAILogicEnabled(Character, AIC, true);
			}
			else
			{
				SetAILogicEnabled(Character, AIC, false);
			}

			// ANIMATIONS
			if ((OptimizerEnable) & (1 << static_cast<uint8>(EAIOptimizerFlags::Animations)))
			{
				Character->GetMesh()->bPauseAnims = false;
				Character->GetMesh()->SetComponentTickInterval(0.f);
			}
			else
			{
				Character->GetMesh()->bPauseAnims = true;
				Character->GetMesh()->SetComponentTickInterval(0.5f);
			}

			// COLLISION
			if ((OptimizerEnable) & (1 << static_cast<uint8>(EAIOptimizerFlags::Collision)))
			{
				Character->SetActorEnableCollision(true);
			}
			else
			{
				Character->SetActorEnableCollision(false);
			}

			// MOVEMENT COMPONENT
			if ((OptimizerEnable) & (1 << static_cast<uint8>(EAIOptimizerFlags::MovementComponent)))
			{
				SetCharacterMovementEnabled(Character, true);
			}
			else
			{
				SetCharacterMovementEnabled(Character, false);
			}

			// SHADOWS
			if ((OptimizerEnable) & (1 << static_cast<uint8>(EAIOptimizerFlags::Shadows)))
			{
				Character->GetMesh()->SetCastShadow(true);
			}
			else
			{
				Character->GetMesh()->SetCastShadow(false);
			}

			// ACTOR TICK
			if ((OptimizerEnable) & (1 << static_cast<uint8>(EAIOptimizerFlags::ActorTick)))
			{
				Character->SetActorTickEnabled(true);
			}
			else
			{
				Character->SetActorTickEnabled(false);
			}

			// VISIBILITY
			if ((OptimizerEnable) & (1 << static_cast<uint8>(EAIOptimizerFlags::Visibility)))
			{
				Character->SetActorHiddenInGame(false);
			}
			else
			{
				Character->SetActorHiddenInGame(true);
			}
		}
	}
}

void UAIOptimizer::OptimizerChecker()
{
	GetWorld()->GetTimerManager().SetTimer(LayerCheckLoopTimer, this, &UAIOptimizer::LayerCheckLoop, 0.5f, true);
}

void UAIOptimizer::LayerCheckLoop()
{
	int32 LayerNum = DistanceLayer();

	switch (LayerNum)
	{
	case 0:
		OptimizerSetting(0);
		break;

	case 1:
		OptimizerSetting(28);
		break;

	case 2:
		OptimizerSetting(31);
		break;

	case 3:
		OptimizerSetting(127);
		break;

	case -1:
		break;

	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not valid Layer."));
		break;
	}
}

int32 UAIOptimizer::DistanceLayer()
{
	FVector AILotation;

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		AILotation = OwnerCharacter->GetActorLocation();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not"));
		return -1;
	}

	float ClosetPlayerDistanceSquared = FLT_MAX;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	float CurrentPlayerDistanceSquared = UKismetMathLibrary::Vector_DistanceSquared(AILotation, PlayerLocation);

	if (CurrentPlayerDistanceSquared < ClosetPlayerDistanceSquared)
	{
		ClosetPlayerDistanceSquared = CurrentPlayerDistanceSquared;
	}

	if (ClosetPlayerDistanceSquared < LayerMax * LayerMax)
	{
		if (ClosetPlayerDistanceSquared < LayerLong * LayerLong)
		{
			if (ClosetPlayerDistanceSquared < LayerMiddle * LayerMiddle)
			{
				if (ClosetPlayerDistanceSquared < LayerShort * LayerShort)
				{
					return 3;
				}

				return 2;
			}

			return 1;
		}
		return 0;
	}

	return -1;
}

void UAIOptimizer::OptimizerCheckerStop()
{
	GetWorld()->GetTimerManager().ClearTimer(LayerCheckLoopTimer);

	OptimizerSetting(0);
}
