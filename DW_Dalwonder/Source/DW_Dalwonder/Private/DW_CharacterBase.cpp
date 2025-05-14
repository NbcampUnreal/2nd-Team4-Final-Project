#include "DW_CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "DW_PlayerController.h"
#include "Monster/DW_MonsterBase.h"
#include "Monster/BossMonster/DW_BossMonsterBaseInterface.h"
#include "Monster/DW_MonsterBaseInterface.h"
#include "Animations/AnimInstance/DW_AnimInstance.h"

ADW_CharacterBase::ADW_CharacterBase()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void ADW_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADW_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ADW_CharacterBase::PlayAttackMontage);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ADW_PlayerController* PlayerController = Cast<ADW_PlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::Move);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::Look);
			}
			
			if (PlayerController->JumpAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Started,
					this,
					&ADW_CharacterBase::StartJump);
	
				EnhancedInputComponent->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ADW_CharacterBase::StopJump);
			}
			
		}
	}
}

void ADW_CharacterBase::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	if (!bCanControl) return;

	FVector2D MoveInput = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ADW_CharacterBase::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ADW_CharacterBase::StartJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void ADW_CharacterBase::StopJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		StopJumping();
	}
}

void ADW_CharacterBase::SetCombatState(ECharacterCombatState NewState)
{
	CurrentCombatState = NewState;

	UE_LOG(LogTemp, Log, TEXT("전투 상태 변경: %s"), *UEnum::GetValueAsString(NewState));
}

void ADW_CharacterBase::PlayAttackMontage()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
}

float ADW_CharacterBase::TakeDamage
	(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser
	)
{
	ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(DamageCauser);

	if (bIsInvincible)
	{
		UE_LOG(LogTemp, Log, TEXT("무적"));
		return 0;
	}
	if (IsValid(Monster))
	{
		// 몬스터가 패링 가능한 상태이고, 캐릭터의 State가 Parrying일 때
		if (Monster->GetCanParry() && CurrentCombatState == ECharacterCombatState::Parrying)
		{
			Monster->Parried();
			PlayAnimMontage(ParryMontage);
		}
		else
		{
			//데미지 받는 로직
		}
	}
	else
	{
		//데미지 받는 로직
	}
	return 0;
}

void ADW_CharacterBase::SetParrying(bool bNewParrying)
{
	if (bIsParrying == bNewParrying)
		return;
	
	bIsParrying = bNewParrying;
	
	if (bNewParrying)
	{
		CurrentCombatState = ECharacterCombatState::Parrying;

		UE_LOG(LogTemp, Log, TEXT("패링 시작"));
	}
	else
	{
		CurrentCombatState = ECharacterCombatState::Idle;

		UE_LOG(LogTemp, Log, TEXT("패링 시작"));
	}
}

void ADW_CharacterBase::SetGuarding(bool bNewGuarding)
{
	if (bIsGuarding == bNewGuarding)
		return;

	bIsGuarding = bNewGuarding;

	if (UDW_AnimInstance* AnimInst = Cast<UDW_AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInst->bIsGuarding = bNewGuarding;
	}

	if (bNewGuarding)
	{
		CurrentCombatState = ECharacterCombatState::Guarding;

		UE_LOG(LogTemp, Log, TEXT("가드 시작"));
	}
	else
	{
		CurrentCombatState = ECharacterCombatState::Idle;
		UE_LOG(LogTemp, Log, TEXT("가드 종료"));
	}
}

void ADW_CharacterBase::SetInvincible(bool bNewInvincible)
{
	if (bIsInvincible == bNewInvincible)
		return;

	bIsInvincible = bNewInvincible;

	if (bNewInvincible)
	{
		UE_LOG(LogTemp, Log, TEXT("무적 상태 ON"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("무적 상태 OFF"));
	}
}

void ADW_CharacterBase::StartGuard()
{
	SetGuarding(true);
	PlayAnimMontage(GuardMontage);
}

void ADW_CharacterBase::EndGuard()
{
	SetGuarding(false);
}

void ADW_CharacterBase::KnockBackCharacter()
{
	BlockCharacterControl(false);
	
	const float KnockBackMultiplier = 50.f;
	const FVector KnockBackDirection = -GetActorForwardVector() * KnockBackMultiplier;
	
	LaunchCharacter(KnockBackDirection, true, true);
	if (IsValid(KnockBackMontage) == true)
	{
		float KnockBackLength = KnockBackMontage->GetPlayLength();
		FTimerHandle KnockBackTimerHandle;
		GetWorldTimerManager().SetTimer(KnockBackTimerHandle, FTimerDelegate::CreateLambda([&]
		{
			BlockCharacterControl(true);
		}
		), KnockBackLength, false, 0.f);
		PlayAnimMontage(KnockBackMontage);
	}
}

void ADW_CharacterBase::BlockCharacterControl(bool bShouldBlock)
{
	bCanControl = bShouldBlock;
}
