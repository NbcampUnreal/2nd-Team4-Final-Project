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
		UE_LOG(LogTemp, Warning, TEXT("Unvalid OwnerCharacter or OwnerCharacterMovementComponent!"));
		return;
	}
	
	Velocity = OwnerCharacterMovementComponent->Velocity;
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, GetOwningActor()->GetActorRotation());
	GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.f).Size();
	bShouldMove = ((OwnerCharacterMovementComponent->GetCurrentAcceleration().IsNearlyZero()) == false) && (GroundSpeed > 3.f);
	bIsFalling = OwnerCharacterMovementComponent->IsFalling();
	bShouldCombat = OwnerCharacter->CurrentCombatState != ECharacterCombatState::Idle;
	bIsLockOn = OwnerCharacter->bIsLockOn;

	//@TODO : 여기 아래에 있는 로직을 CharacterBase의 Tick 또는 Timer 이용하여 옮겨야 함
	if (bIsLockOn == true)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
	}
	else
	{
		OwnerCharacter->bUseControllerRotationYaw = false;
	}
}
