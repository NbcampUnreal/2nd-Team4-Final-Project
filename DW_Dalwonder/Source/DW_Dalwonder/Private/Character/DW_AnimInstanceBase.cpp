#include "Character/DW_AnimInstanceBase.h"
#include "Character/DW_CharacterBase.h"
#include "Character/ECharacterCombatState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UDW_AnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<ADW_CharacterBase>(GetOwningActor());
	if (IsValid(OwnerCharacter) == true)
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UDW_AnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(OwnerCharacter) == false || IsValid(OwnerCharacterMovementComponent) == false)
	{
		return;
	}
	
	Velocity = OwnerCharacterMovementComponent->Velocity;
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, OwnerCharacter->GetActorRotation());
	GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.f).Size();
	bShouldMove = ((OwnerCharacterMovementComponent->GetCurrentAcceleration().IsNearlyZero()) == false) && (GroundSpeed > 3.f);
	bIsFalling = OwnerCharacterMovementComponent->IsFalling();
	bShouldCombat = OwnerCharacter->bIsOnCombat;
	bIsLockOn = OwnerCharacter->bIsLockOn;
	bIsDodging = OwnerCharacter->CurrentCombatState == ECharacterCombatState::Dodging;
}
