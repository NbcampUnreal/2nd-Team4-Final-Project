#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacterCombatState.h"
#include "Monster/BossMonster/DW_BossMonsterBaseInterface.h"
#include "Monster/DW_MonsterBaseInterface.h"
#include "GameFramework/Character.h"
#include "DW_CharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class DW_DALWONDER_API ADW_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADW_CharacterBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	ECharacterCombatState CurrentCombatState = ECharacterCombatState::Parrying;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCombatState(ECharacterCombatState NewState);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PlayAttackMontage();

	virtual float TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser
) override;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetParrying(bool bNewParrying);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetGuarding(bool bNewParrying);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetInvincible(bool bNewParrying);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartGuard();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndGuard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* Weapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsParrying = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsGuarding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsInvincible = false;

};
