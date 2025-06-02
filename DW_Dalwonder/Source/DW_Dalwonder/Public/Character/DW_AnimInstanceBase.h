#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DW_AnimInstanceBase.generated.h"

class ADW_CharacterBase;
class UCharacterMovementComponent;

UCLASS()
class DW_DALWONDER_API UDW_AnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<ADW_CharacterBase> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldMove = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFalling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldCombat = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsLockOn = false;
};
