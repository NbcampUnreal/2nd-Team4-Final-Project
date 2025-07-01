#include "Character/DW_CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "DW_GameInstance.h"
#include "EngineUtils.h"
#include "Character/DW_PlayerController.h"
#include "Character/DW_AnimInstanceBase.h"
#include "Character/CharacterStatComponent.h"
#include "Monster/DW_MonsterBase.h"
#include "Item/WorldItemActor.h"
#include "NiagaraFunctionLibrary.h"
#include "UI/Widget/HUDWidget.h"
#include "DW_GmBase.h"
#include "Components/CapsuleComponent.h"
#include "Item/ItemDataManager.h"
#include "DW_InteractInterface.h"
#include "HeadMountedDisplayTypes.h"
#include "KismetAnimationLibrary.h"
#include "Character/CharacterArmorComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "UI/Widget/LockOnWidget.h"
#include "Tracks/MovieSceneMaterialTrack.h"
#include "Item/Interactionprompt.h"
#include "UI/Widget/SettingsManager.h"
#include "Components/WidgetComponent.h"


ADW_CharacterBase::ADW_CharacterBase()
{
	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("StatComponent"));
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 200.f;
	SpringArm->SetRelativeRotation(FRotator(0.f, 20.f, 0.f));
	SpringArm->SocketOffset = FVector(0.f, 60.f, 70.f);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 105.f;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->MaxWalkSpeed = (StatComponent->GetTotalWalkSpeed());

	ArmorComponent = CreateDefaultSubobject<UCharacterArmorComponent>(TEXT("ArmorComponent"));

	Vehicle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Horse"));
	Vehicle->SetupAttachment(RootComponent);
	Reins = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Reins"));
	Reins->SetupAttachment(Vehicle);
	Saddle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Saddle"));
	Saddle->SetupAttachment(Vehicle);
	SaddleBelts = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SaddleBelts"));
	SaddleBelts->SetupAttachment(Vehicle);
	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(Vehicle);
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	SkillComponent = CreateDefaultSubobject<UDW_SkillComponent>(TEXT("SkillComponent"));
	AttributeComponent = CreateDefaultSubobject<UDW_AttributeComponent>(TEXT("AttributeComponent"));

	// SceneCaptureComponent 초기화
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(RootComponent);

	SceneCaptureComponent->SetRelativeLocation(FVector(250.f, 0.f, 30.f));  // 위치 조정
	SceneCaptureComponent->SetRelativeRotation(FRotator(-10.f, 180.f, 0.f)); // 캐릭터를 바라보게 회전

	// 기본 렌더링 세팅
	SceneCaptureComponent->bCaptureEveryFrame = true;
	SceneCaptureComponent->bCaptureOnMovement = false;
	SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Perspective;
	SceneCaptureComponent->FOVAngle = 45.f;

	// 최적화 설정
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->bCaptureOnMovement = false;
	SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

	// 퀘스트 매니저
	QuestManager = CreateDefaultSubobject<UQuestManagerComponent>(TEXT("QuestManager"));

	Helmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet"));
	Helmet->SetupAttachment(GetMesh());
	Armor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Armor"));
	Armor->SetupAttachment(GetMesh());
	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	Pants->SetupAttachment(GetMesh());
	Glove = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Glove"));
	Glove->SetupAttachment(GetMesh());
	Boots = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boot"));
	Boots->SetupAttachment(GetMesh());
}

void ADW_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UpdateSkeletalMesh();

	GetWorld()->GetTimerManager().SetTimer  //아이템 업그레이드 타이머
	(
		ItemScanTimerHandle,
		this,
		&ADW_CharacterBase::UpdateClosestItem,
		0.1f,         
		true          
	);

	GetWorldTimerManager().SetTimer(
		FootstepTraceTimerHandle,
		this,
		&ADW_CharacterBase::UpdateFootstepSurface,
		0.01f,   // 주기
		true     // 반복 여부
	);

	// HUD 타이머 설정 (0.1초 간격)
	GetWorld()->GetTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ADW_CharacterBase::UpdateHUD,
		0.1f,
		true  // 반복 여부
	);

	if (RenderTarget && SceneCaptureComponent)
	{
		SceneCaptureComponent->TextureTarget = RenderTarget;
		SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

		// 자식 액터들의 컴포넌트도 따로 추가
		TArray<AActor*> ChildActors;
		GetAttachedActors(ChildActors, true); // true = recursive

		for (AActor* Child : ChildActors)
		{
			if (!Child) continue;

			TArray<UActorComponent*> Components = Child->GetComponents().Array();
			for (UActorComponent* Comp : Components)
			{
				if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Comp))
				{
					SceneCaptureComponent->ShowOnlyComponent(Primitive);
				}
			}
		}

		// 자신만 보여주도록 설정 (배경 안 보이게)
		SceneCaptureComponent->ShowOnlyActorComponents(this);

		// 캡처
		SceneCaptureComponent->CaptureScene();
	}

}

void ADW_CharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorldTimerManager().ClearTimer(BlockTimer);
	GetWorldTimerManager().ClearTimer(DodgeTimer);
	GetWorldTimerManager().ClearTimer(InvincibleTimer);
	GetWorldTimerManager().ClearTimer(IdleStateTimer);
}

void ADW_CharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = GetMesh()->GetAnimInstance();
	SetVehicleVisibility(false);
}

void ADW_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	// {
	// 	// 컨트롤러로 토글 요청
	// 	if (ADW_PlayerController* PC = Cast<ADW_PlayerController>(GetController()))
	// 	{
	// 		EIC->BindAction(PC->ESCAction, ETriggerEvent::Started, PC, &ADW_PlayerController::ToggleESCMenu);
	// 	}
	// }
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ADW_PlayerController* PlayerController = Cast<ADW_PlayerController>(GetController()))
		{
			// EnhancedInputComponent->BindAction(
			// 	PlayerController->ESCAction,
			// 	ETriggerEvent::Started,
			// 	PlayerController,
			// 	&ADW_PlayerController::ToggleESCMenu
			// );
			if (PlayerController->MoveForwardAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->MoveForwardAction,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::MoveForward);
			}
			if (PlayerController->MoveBackwardAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->MoveBackwardAction,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::MoveBackward);
			}
			if (PlayerController->MoveLeftAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->MoveLeftAction,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::MoveLeft);
			}
			if (PlayerController->MoveRightAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->MoveRightAction,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::MoveRight);
			}

			if (PlayerController->LookUpAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->LookUpAction,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::LookUp);
			}

			if (PlayerController->TurnAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->TurnAction,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::Turn);
			}
			
			if (PlayerController->JumpAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::StartJump);
	
				EnhancedInputComponent->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ADW_CharacterBase::StopJump);
			}

			if (PlayerController->AttackAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->AttackAction,
					ETriggerEvent::Started,
					this,
					&ADW_CharacterBase::Attack);
			}

			if (PlayerController->GuardAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->GuardAction,
					ETriggerEvent::Started,
					this,
					&ADW_CharacterBase::StartGuard);

				EnhancedInputComponent->BindAction(
					PlayerController->GuardAction,
					ETriggerEvent::Completed,
					this,
					&ADW_CharacterBase::EndGuard);
			}

			if (PlayerController->DodgeAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->DodgeAction,
					ETriggerEvent::Started,
					this,
					&ADW_CharacterBase::Dodge);
			}

			if (PlayerController->LockonAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->LockonAction,
					ETriggerEvent::Started,
					this,
					&ADW_CharacterBase::Lockon);
			}

			if (PlayerController->SkillAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->SkillAction,
					ETriggerEvent::Started,
					this,
					&ADW_CharacterBase::UseActiveSkill);
			}

			if (PlayerController->UseSkill1Action)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->UseSkill1Action,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::UseActiveSkillSlot1);
			}

			if (PlayerController->UseSkill2Action)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->UseSkill2Action,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::UseActiveSkillSlot2);
			}

			if (PlayerController->UseSkill3Action)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->UseSkill3Action,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::UseActiveSkillSlot3);
			}

			if (PlayerController->RideAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->RideAction,
					ETriggerEvent::Triggered,
					this,
					&ADW_CharacterBase::Ride);
			}

			if (PlayerController->InteractAction)
			{
#if WITH_EDITOR
#endif

				EnhancedInputComponent->BindAction(
					PlayerController->InteractAction,
					ETriggerEvent::Started,
					this,
					&ADW_CharacterBase::Interact);

#if WITH_EDITOR
#endif
			}
			else
			{
#if WITH_EDITOR
#endif
			}
		}
	}
}
void ADW_CharacterBase::MoveForward(const FInputActionValue& Value)
{
	if (!Controller || !bCanControl) return;

	const float AxisValue = Value.Get<float>();
	if (FMath::IsNearlyZero(AxisValue)) return;

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, AxisValue);
}

void ADW_CharacterBase::MoveBackward(const FInputActionValue& Value)
{
	if (!Controller || !bCanControl) return;

	const float AxisValue = -Value.Get<float>(); // 반전
	if (FMath::IsNearlyZero(AxisValue)) return;

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, AxisValue);
}

void ADW_CharacterBase::MoveLeft(const FInputActionValue& Value)
{
	if (!Controller || !bCanControl) return;

	const float AxisValue = -Value.Get<float>(); // 반전
	if (FMath::IsNearlyZero(AxisValue)) return;

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, AxisValue);
}

void ADW_CharacterBase::MoveRight(const FInputActionValue& Value)
{
	if (!Controller || !bCanControl) return;

	const float AxisValue = Value.Get<float>();
	if (FMath::IsNearlyZero(AxisValue)) return;

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, AxisValue);
}


void ADW_CharacterBase::LookUp(const FInputActionValue& Value)
{
	if (bIsLockOn) return;

	float YInput = Value.Get<float>();

	float Sensitivity = 1.f;
	if (UWorld* World = GetWorld())
	{
		if (UDW_GameInstance* GI = Cast<UDW_GameInstance>(World->GetGameInstance()))
		{
			if (const USettingsManager* SM = GI->GetSettingsManager())
			{
				Sensitivity = SM->GetMouseSensitivity();
			}
		}
	}

	AddControllerPitchInput(-YInput * Sensitivity);
}

void ADW_CharacterBase::Turn(const FInputActionValue& Value)
{
	if (bIsLockOn) return;

	float XInput = Value.Get<float>();

	float Sensitivity = 1.f;
	if (UWorld* World = GetWorld())
	{
		if (UDW_GameInstance* GI = Cast<UDW_GameInstance>(World->GetGameInstance()))
		{
			if (const USettingsManager* SM = GI->GetSettingsManager())
			{
				Sensitivity = SM->GetMouseSensitivity();
			}
		}
	}

	AddControllerYawInput(XInput * Sensitivity);
}

void ADW_CharacterBase::StartJump(const FInputActionValue& Value)
{
	if (!bCanControl) return;
	
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void ADW_CharacterBase::StopJump(const FInputActionValue& Value)
{
	if (!bCanControl) return;
	
	if (Value.Get<bool>())
	{
		StopJumping();
	}
}

void ADW_CharacterBase::Attack(const FInputActionValue& Value)
{
	if (CurrentCombatState == ECharacterCombatState::Dead ||
		CurrentCombatState == ECharacterCombatState::Dodging ||
		CurrentCombatState == ECharacterCombatState::Hit ||
		CurrentCombatState == ECharacterCombatState::Parrying) return;
	
	if (Value.Get<bool>())
	{
		StartAttack();
	}
}

void ADW_CharacterBase::Sprint(bool bOnSprint)
{
	if (bOnSprint == true && StatComponent->GetStamina() <= 5.f)
	{
		return;
	}
	
	if (bIsSprinting == bOnSprint)
	{
		return;
	}

	bIsSprinting = bOnSprint;
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Sprint Function Called")));
	
	if (bIsSprinting)
	{
		if (bIsRidingVehicle)
		{
			GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetSprintSpeed() * VehicleSpeedMultiplier;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetSprintSpeed();
		}
		GetCharacterStatComponent()->ConsumeStamina(2.f);
	}
	else
	{
		if (bIsRidingVehicle)
		{
			GetCharacterMovement()->MaxWalkSpeed = (StatComponent->GetTotalWalkSpeed()) * VehicleSpeedMultiplier;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = (StatComponent->GetTotalWalkSpeed());
		}
		GetCharacterStatComponent()->StopConsumeStamina();
		GetCharacterStatComponent()->StartStaminaRegen();
	}
}

void ADW_CharacterBase::Dodge(const FInputActionValue& Value)
{
	if (CurrentCombatState == ECharacterCombatState::Dodging)
	{
		return;
	}
	
	if (Value.Get<bool>())
	{
		if (GetCharacterStatComponent()->GetStamina() < 10.f)
		{
			return;
		}

		GetCharacterStatComponent()->SetStamina(GetCharacterStatComponent()->GetStamina() - 10.f);
		SetCombatState(ECharacterCombatState::Dodging);

		FVector InputVector = GetLastMovementInputVector();
		UDW_AnimInstanceBase* DW_AnimInstance = Cast<UDW_AnimInstanceBase>(AnimInstance);
		if (!InputVector.IsNearlyZero())
		{
			DW_AnimInstance->DodgeDirection = UKismetAnimationLibrary::CalculateDirection(InputVector, GetActorRotation());
		}
		else
		{
			DW_AnimInstance->DodgeDirection = 0.f;
		}
		bIsInvincible = true;
		BlockCharacterControl(true);

		AnimInstance->RootMotionMode = ERootMotionMode::RootMotionFromEverything;

		GetWorld()->GetTimerManager().SetTimer(DodgeTimer, FTimerDelegate::CreateLambda([&]
		{
			SetCombatState(ECharacterCombatState::Idle);
			BlockCharacterControl(false);
			AnimInstance->RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
		}), 1.34f, false);
		
		GetWorld()->GetTimerManager().SetTimer(InvincibleTimer, FTimerDelegate::CreateLambda([&]
		{
			bIsInvincible = false;
		}), InvincibleDuration, false);
	}
}

void ADW_CharacterBase::Lockon(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		ToggleLockOn();
	}
}

void ADW_CharacterBase::Ride(const FInputActionValue& Value)
{
	if (!bCanControl) return;
	
	if (!bCanRideVehicle)
	{
		return;
	}
	
	if (Value.Get<bool>())
	{
		RideVehicle(!bIsRidingVehicle);
	}
}

void ADW_CharacterBase::PlayMontage(UAnimMontage* Montage, int32 SectionIndex)
{
	if (CurrentCombatState == ECharacterCombatState::Dead) return;
	
	BlockCharacterControl(true);
	
	if (IsValid(AnimInstance))
	{
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindUObject(this, &ADW_CharacterBase::OnMontageEnded);
		
		if (!IsValid(Montage) || SectionIndex >= Montage->GetNumSections()) return;
		
		if (SectionIndex != -1)
		{
			FName SectionName = Montage->GetSectionName(SectionIndex);
			if (AnimInstance->Montage_IsPlaying(Montage) == false)
			{
				if (CurrentCombatState == ECharacterCombatState::Attacking || CurrentCombatState == ECharacterCombatState::ComboWindow)
				{
					AnimInstance->Montage_Play(Montage, StatComponent->GetTotalAttackSpeed());
				}
				else
				{
					AnimInstance->Montage_Play(Montage);
				}
			}
			AnimInstance->Montage_JumpToSection(SectionName);
			AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, Montage);
		}
		else
		{
			if (AnimInstance->Montage_IsPlaying(Montage) == false)
			{
				if (CurrentCombatState == ECharacterCombatState::Attacking || CurrentCombatState == ECharacterCombatState::ComboWindow)
				{
					AnimInstance->Montage_Play(Montage, StatComponent->GetTotalAttackSpeed());
				}
				else
				{
					AnimInstance->Montage_Play(Montage);
				}
			}
			AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, Montage);
		}
	}
}

void ADW_CharacterBase::SetWeaponType(int32 NewWeaponType)
{
	if (WeaponType == NewWeaponType)
	{
		return;
	}
	
	WeaponType = NewWeaponType;
	GetMesh()->SetAnimInstanceClass(AnimInstanceArray[WeaponType]);
	AnimInstance = GetMesh()->GetAnimInstance();
}

void ADW_CharacterBase::UpdateSkeletalMesh()
{
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents = { Helmet, Armor, Glove, Pants, Boots };

	for (USkeletalMeshComponent* Part : SkeletalMeshComponents)
	{
		if (IsValid(Part))
		{
			Part->SetLeaderPoseComponent(GetMesh());
			
		}
	}
}

void ADW_CharacterBase::SetCombatState(ECharacterCombatState NewState)
{
	CurrentCombatState = NewState;
#if WITH_EDITOR
	UE_LOG(LogTemp, Log, TEXT("전투 상태 변경: %s"), *UEnum::GetValueAsString(NewState));
#endif

	if (CurrentCombatState != ECharacterCombatState::Idle && CurrentCombatState != ECharacterCombatState::Dodging)
	{
		bIsOnCombat = true;
		SetIdleState();
	}
}

void ADW_CharacterBase::StartAttack()
{
	if (CurrentCombatState != ECharacterCombatState::Idle && CurrentCombatState != ECharacterCombatState::ComboWindow) return;
	
	BlockCharacterControl(true);

	if (GetMovementComponent()->IsFalling())
	{
		check(IsValid(FallingAttackMontage[WeaponType]));
		SetCombatState(ECharacterCombatState::Attacking);
		PlayMontage(FallingAttackMontage[WeaponType]);
	}
	else if (bIsGuarding)
	{
		check(IsValid(GuardAttackMontage[WeaponType]));
		SetCombatState(ECharacterCombatState::Attacking);
		PlayMontage(GuardAttackMontage[WeaponType]);
	}
	else if (bIsSprinting && GetVelocity().Length() > GetCharacterStatComponent()->GetTotalWalkSpeed() && CurrentCombatState != ECharacterCombatState::ComboWindow)
	{
		check(IsValid(SprintAttackMontage[WeaponType]));
		SetCombatState(ECharacterCombatState::Attacking);
		PlayMontage(SprintAttackMontage[WeaponType]);
	}
	else
	{
		check(IsValid(AttackMontage[WeaponType]));

		if (CurrentCombatState == ECharacterCombatState::Idle)
		{
			CurrentComboIndex = 0;
			SetCombatState(ECharacterCombatState::ComboWindow);
			PlayMontage(AttackMontage[WeaponType]);
		}
		else if (CurrentCombatState == ECharacterCombatState::ComboWindow && bCanCombo)
		{
			CurrentComboIndex++;
			if (CurrentComboIndex < AttackMontage[WeaponType]->GetNumSections())
			{
				PlayMontage(AttackMontage[WeaponType], CurrentComboIndex);
				bCanCombo = false;
			}
		}
	}
}

void ADW_CharacterBase::CancelAttack()
{
	// 현재 재생 중인 모든 몽타주를 중단
	AnimInstance->Montage_Stop(0.2f);
	
	// 튕김 애니메이션 재생
	if (IsValid(CancelAttackMontage[WeaponType]))
	{
		PlayMontage(CancelAttackMontage[WeaponType]);
	}
}

void ADW_CharacterBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (CurrentCombatState == ECharacterCombatState::Dead) return;
	
	if (Montage == AttackMontage[WeaponType])
	{
		CurrentComboIndex = 0;
		bCanCombo = false;
	}

	if (AnimInstance->Montage_IsPlaying(nullptr))
	{
		return;
	}
	
	SetCombatState(ECharacterCombatState::Idle);
	BlockCharacterControl(false);
}

float ADW_CharacterBase::TakeDamage(float DamageAmount,FDamageEvent const& DamageEvent,AController* EventInstigator,AActor* DamageCauser)
{
	float ActualDamage = DamageAmount;

	if (CurrentCombatState == ECharacterCombatState::Dead)
	{
		return 0.f;
	}
	
	// 캐릭터가 무적 상태일 때
	if (bIsInvincible)
	{
		ActualDamage = 0.f;
		return ActualDamage;
	}

	// 몬스터가 패링 가능한 상태이고, 캐릭터의 State가 Parrying일 때
	ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(DamageCauser);
	if (DamageCauser->Implements<IDW_MonsterBaseInterface::UClassType>())
	{
		if (Monster->GetCanParry() && CurrentCombatState == ECharacterCombatState::Parrying)
		{
			Monster->Parried();
			PlayMontage(ParryMontage[WeaponType]);
			ActualDamage = 0.f;
			return ActualDamage;
		}
	}
	else
	{
		if (CurrentCombatState == ECharacterCombatState::Parrying)
		{
			PlayMontage(ParryMontage[WeaponType]);
			ActualDamage = 0.f;
			return ActualDamage;
		}
	}
	
	// 캐릭터가 가드 상태일 때
	if (bIsGuarding)
	{
		ActualDamage /= 0.5f;
	}
	else
	{
		float KnockBackAmount = (GetCharacterStatComponent()->GetTotalMaxHealth()) * 0.3f;
		if (DamageAmount > KnockBackAmount)
		{
			KnockBackCharacter();
		}
		else if (AnimInstance->Montage_IsPlaying(KnockBackMontage[WeaponType]))
		{
			SetCombatState(ECharacterCombatState::Hit);
			int32 HitSectionNum = HitMontage[WeaponType]->GetNumSections();
			int32 RandomHitSectionNum = FMath::RandRange(0, HitSectionNum - 1);
			PlayMontage(HitMontage[WeaponType], RandomHitSectionNum);
		}
	}

	// 데미지 입을경우 UI 전부 닫기
	if (ActualDamage > 0.f)
	{
		if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
		{
			while (GM->GetPopupWidgetCount() > 0)
			{
				GM->CloseLastPopupUI();
			}
		}

		if (ADW_PlayerController* PC = Cast<ADW_PlayerController>(GetController()))
		{
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());
			this->EnableInput(PC);
			PC->ESCMenuWidgetInstance = nullptr;
			PC->bIsESCMenuOpen = false;
		}
	}

	// 체력 감소 로직
	StatComponent->SetHealth(StatComponent->GetHealth() - ActualDamage);

	if (FMath::IsNearlyZero(StatComponent->GetHealth()))
	{
		Dead();
	}
	
	return ActualDamage;
}

void ADW_CharacterBase::SetParrying(bool bIsParrying)
{
	if (bIsParrying)
	{
		SetCombatState(ECharacterCombatState::Parrying);
	}
	else
	{
		SetCombatState(ECharacterCombatState::Idle);
	}
}

void ADW_CharacterBase::SetGuarding(bool bNewGuarding)
{
	if (bIsGuarding == bNewGuarding)
	{
		return;
	}
	
	bIsGuarding = bNewGuarding;

	if (bIsGuarding)
	{
		GetCharacterStatComponent()->ConsumeStamina(2.f);
		PlayMontage(GuardMontage[WeaponType]);
	}
	else
	{
		GetCharacterStatComponent()->StopConsumeStamina();
		GetCharacterStatComponent()->StartStaminaRegen();
		AnimInstance->Montage_Stop(0.25f, GuardMontage[WeaponType]);
	}
}

void ADW_CharacterBase::SetInvincible(bool bNewInvincible)
{
	if (bIsInvincible == bNewInvincible)
		return;

	bIsInvincible = bNewInvincible;
}

void ADW_CharacterBase::StartGuard()
{
	if (GetCharacterStatComponent()->GetStamina() < 5.f)
	{
		return;
	}

	if (CurrentCombatState == ECharacterCombatState::Idle)
	{
		SetGuarding(true);
	}
}

void ADW_CharacterBase::EndGuard()
{
	SetGuarding(false);
}

void ADW_CharacterBase::UseActiveSkill()
{
	//@TODO : SkillComponent 와 연계해서 액티브 스킬 사용 만들기 -> Montage, 스킬 Array, 자원 소모 구현
	// SkillComponent->GetActiveSkillArray();

	FTimerHandle SkillUseTimer;
	GetWorld()->GetTimerManager().SetTimer(SkillUseTimer, FTimerDelegate::CreateLambda([&]
	{
		// TODO
	}), 2.f, false);
}

void ADW_CharacterBase::UseActiveSkillSlot1()
{
	SetCombatState(ECharacterCombatState::Attacking);
	StatComponent->SetStamina(StatComponent->GetStamina() - 10.f);
	PlayMontage(SkillMontage[WeaponType][0]);
}

void ADW_CharacterBase::UseActiveSkillSlot2()
{
	SetCombatState(ECharacterCombatState::Attacking);
	StatComponent->SetStamina(StatComponent->GetStamina() - 10.f);
	PlayMontage(SkillMontage[WeaponType][1]);
}

void ADW_CharacterBase::UseActiveSkillSlot3()
{
	SetCombatState(ECharacterCombatState::Attacking);
	StatComponent->SetStamina(StatComponent->GetStamina() - 10.f);
	PlayMontage(SkillMontage[WeaponType][2]);
}

void ADW_CharacterBase::KnockBackCharacter()
{
	if (CurrentCombatState != ECharacterCombatState::Dead)
	{
		SetCombatState(ECharacterCombatState::Hit);
	}
	
	const float KnockBackMultiplier = 50.f;
	const FVector KnockBackDirection = -GetActorForwardVector() * KnockBackMultiplier;
	
	LaunchCharacter(KnockBackDirection, true, true);
	if (IsValid(KnockBackMontage[WeaponType]) == true)
	{
		if (AnimInstance->Montage_IsPlaying(KnockBackMontage[WeaponType]))
		{
			PlayMontage(KnockBackMontage[WeaponType], 1);
		}
		else
		{
			PlayMontage(KnockBackMontage[WeaponType]);
		}
	}
}

void ADW_CharacterBase::BlockCharacterControl(bool bShouldBlock, float Length)
{
	bCanControl = !bShouldBlock;

	if (!FMath::IsNearlyZero(Length))
	{
		check(IsValid(GetWorld()));
	
		GetWorldTimerManager().SetTimer(BlockTimer, FTimerDelegate::CreateLambda([&]()
			{
				bCanControl = bShouldBlock;
			}), Length, false);
	}
}

void ADW_CharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (!GetCharacterMovement()->IsFalling()) return;
	
	float Velocity = GetVelocity().Z * -1.f;
	
	if (Velocity <= FallVelocityLimit)
	{
		return;
	}

	float FallDamage = Velocity / 100.f;

	UGameplayStatics::ApplyDamage(this, FallDamage, GetController(), this, UDamageType::StaticClass());
}

void ADW_CharacterBase::Dead()
{
	DisableInput(Cast<APlayerController>(GetController()));
	StatComponent->StopConsumeHealth();
	StatComponent->StopConsumeStamina();
	bCanRideVehicle = false;
	
	if (CurrentCombatState == ECharacterCombatState::Attacking)
	{
		PlayMontage(DeadMontage[WeaponType], 1);
	}
	else
	{
		PlayMontage(DeadMontage[WeaponType]);
	}

	SetCombatState(ECharacterCombatState::Dead);
	
	if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->ShowResultUI("YOU DIED");
	};
}

void ADW_CharacterBase::SetIdleState()
{
	GetWorldTimerManager().ClearTimer(IdleStateTimer);

	GetWorldTimerManager().SetTimer(IdleStateTimer, FTimerDelegate::CreateLambda([&]
	{
		bIsOnCombat = false;
	}), 10.f, false);
}

void ADW_CharacterBase::Interact()
{
	FVector Start = GetActorLocation() + FVector(0.f, 0.f, BaseEyeHeight);
	FRotator ControlRot = GetControlRotation();
	FVector End = Start + ControlRot.Vector() * InteractDistance;

	const float realSphereRadius = 90.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult Hit;
	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(realSphereRadius),
		Params
	);

#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), End, realSphereRadius, 12, FColor::Green, false, 1.0f);
#endif

	if (bHit && Hit.bBlockingHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<UDW_InteractInterface>())
		{
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("[Interact] 맞은 액터: %s"), *HitActor->GetName());
#endif
			IDW_InteractInterface::Execute_Interact(HitActor, this);
		}
		else
		{
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("[Interact] 인터페이스 미구현 액터: %s"), *GetNameSafe(HitActor));
#endif
		}
	}
	else
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("[Interact] 아무것도 맞지 않음."));
#endif
	}

	if (CurrentItem)
	{

		UItemBase* Data = CurrentItem->ItemBase; // 아이템 정보 가져오기
		int32 ItemCount = CurrentItem->GetItemCount(); // 아이템 개수 가져오기
		bool bAdded = InventoryComponent->AddItem(Data, ItemCount);
		if (bAdded)
		{
			CurrentItem->Destroy();
			CurrentItem = nullptr;
		}
		else
		{
			CurrentItem->SetItemCount(ItemCount);
			UUserWidget* UserWidgetInstance = CurrentItem->InteractionWidget->GetUserWidgetObject();

			// 가져온 위젯 객체를 UInteractionprompt* 타입으로 캐스팅합니다.
			UInteractionprompt* PromptWidget = Cast<UInteractionprompt>(UserWidgetInstance);

			if (PromptWidget)
			{
				// 성공적으로 캐스팅되었다면 InitializeData 함수를 호출합니다.
				PromptWidget->InitializeData();
				PromptWidget->SoundPlay();
			}
		}
	}
}


void ADW_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Start = GetActorLocation() + FVector(0, 0, 10.f);
	FVector End = Start + GetActorForwardVector() * InteractDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult Hit;
	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(SphereRadius),
		Params
	);

	AActor* NewInteractTarget = nullptr;

	if (bHit && Hit.bBlockingHit && Hit.GetActor())
	{
		if (Hit.GetActor()->Implements<UDW_InteractInterface>())
		{
			NewInteractTarget = Hit.GetActor();

			if (CurrentInteractTarget != NewInteractTarget)
			{
				if (CurrentInteractTarget && CurrentInteractTarget->Implements<UDW_InteractInterface>())
				{
					IDW_InteractInterface::Execute_HideInteractionWidget(CurrentInteractTarget);
				}

				IDW_InteractInterface::Execute_ShowInteractionWidget(NewInteractTarget);
			}
		}
	}

	if (!NewInteractTarget && CurrentInteractTarget)
	{
		if (CurrentInteractTarget->Implements<UDW_InteractInterface>())
		{
			IDW_InteractInterface::Execute_HideInteractionWidget(CurrentInteractTarget);
		}
	}

	CurrentInteractTarget = NewInteractTarget;

	// 디버그 구체
	//DrawDebugSphere(GetWorld(), End, SphereRadius, 12, FColor::Yellow, false, 0.1f);

	// 화면 좌표로 변환하여 UI 업데이트
	if (CurrentInteractTarget)
	{
		FVector ActorLocation = CurrentInteractTarget->GetActorLocation();
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (PlayerController)
		{
			FVector2D ScreenPosition;
			if (PlayerController->ProjectWorldLocationToScreen(ActorLocation, ScreenPosition))
			{
				// 여기서 UI를 화면 상에 표시
				if (InteractionWidget)
				{
					InteractionWidget->SetPositionInViewport(ScreenPosition);
					InteractionWidget->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}

	// 락온 상태에 따라 컨트롤러 로테이션 Yaw 사용 전환 로직
	if (bIsLockOn == true)
	{
		bUseControllerRotationYaw = true;
	}
	else
	{
		bUseControllerRotationYaw = false;
	}
}

void ADW_CharacterBase::AddNearbyItem(AWorldItemActor* Item)
{
	if (Item && !NearbyItems.Contains(Item))
	{
		NearbyItems.Add(Item);
	}
}

void ADW_CharacterBase::RemoveNearbyItem(AWorldItemActor* Item)
{
	if (Item)
	{
		NearbyItems.Remove(Item);
	}
}

void ADW_CharacterBase::UpdateClosestItem()
{
	float ClosestDistance = TNumericLimits<float>::Max();
	AWorldItemActor* ClosestItem = nullptr;

	for (AWorldItemActor* Item : NearbyItems)
	{
		if (!IsValid(Item)) continue;

		float Dist = FVector::Dist(this->GetActorLocation(), Item->GetActorLocation());
		if (Dist < ClosestDistance)
		{
			ClosestDistance = Dist;
			ClosestItem = Item;
		}
	}

	CurrentItem = ClosestItem;
}

void ADW_CharacterBase::UpdateHUD()
{
	if (!StatComponent) return;

	// 컨트롤러에서 HUD 가져오기
	if (ADW_PlayerController* PC = Cast<ADW_PlayerController>(GetController()))
	{
		if (UHUDWidget* HUD = Cast<UHUDWidget>(PC->HUDWidgetInstance))  // 정확한 클래스 캐스팅
		{
			HUD->UpdateHPBar(StatComponent->GetHealth(), StatComponent->GetTotalMaxHealth());
			HUD->UpdateStaminaBar(StatComponent->GetStamina(), StatComponent->GetTotalMaxHealth());
		}
		else {
			//캐스팅 실패시 타이머 초기화
			GetWorld()->GetTimerManager().ClearTimer(HUDUpdateTimerHandle);
		}
		//현재 HP, Stamina만 업데이트중 아이템(물약) 사용시도 필요하면 제작
	}
}

void ADW_CharacterBase::ToggleLockOn()
{
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (bIsLockOn)
	{
		// 🔓 락온 해제
		bIsLockOn = false;
		LockOnTarget = nullptr;
		GetWorldTimerManager().ClearTimer(LockOnRotationTimer);

		if (LockOnWidgetInstance)
		{
			LockOnWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		AActor* Target = FindBestLockOnTarget();
		if (IsValid(Target))
		{
			bIsLockOn = true;
			LockOnTarget = Target;

			if (!LockOnWidgetInstance && IsValid(LockOnWidgetClass))
			{
				LockOnWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LockOnWidgetClass);
				if (LockOnWidgetInstance)
				{
					LockOnWidgetInstance->AddToViewport();
				}
			}

			if (LockOnWidgetInstance)
			{
				LockOnWidgetInstance->SetVisibility(ESlateVisibility::Visible);
			}

			GetWorldTimerManager().SetTimer(
				LockOnRotationTimer,
				this,
				&ADW_CharacterBase::UpdateLockOnRotation,
				0.01f,
				true
			);

			GetWorldTimerManager().SetTimer(
				LockOnMarkerUpdateTimer,
				this,
				&ADW_CharacterBase::UpdateLockOnMarkerPosition,
				0.01f,
				true
			);
		}
	}
}



AActor* ADW_CharacterBase::FindClosestTarget(float MaxDistance)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	AActor* ClosestTarget = nullptr;
	float ClosestDistance = MaxDistance;

	FVector MyLocation = GetActorLocation();
	APlayerController* PC = Cast<APlayerController>(GetController());

	for (TActorIterator<ADW_MonsterBase> It(World); It; ++It)
	{
		ADW_MonsterBase* Monster = *It;

		if (!IsValid(Monster)) continue;

		const float Distance = FVector::Dist(MyLocation, Monster->GetActorLocation());
		if (Distance > ClosestDistance) continue;

		if (IsValid(PC) && !PC->LineOfSightTo(Monster)) continue;

		ClosestDistance = Distance;
		ClosestTarget = Monster;
	}

	return ClosestTarget;
}

void ADW_CharacterBase::RideVehicle(bool bOnRiding)
{
	if (bIsRidingVehicle == bOnRiding)
	{
		return;
	}

	bIsRidingVehicle = bOnRiding;
	float BaseWalkSpeed = StatComponent->GetTotalWalkSpeed();

	if (bIsRidingVehicle)
	{
		SetVehicleVisibility(true);
		PlayMontage(RidingMontage);
		PlayVehicleMontage(RidingHorseMontage);
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * VehicleSpeedMultiplier;
	}
	else
	{
		PlayMontage(GetOffMontage);
		PlayVehicleMontage(GetOffHorseMontage);
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void ADW_CharacterBase::SetVehicleVisibility(bool bOnRiding)
{
	FName TagName(TEXT("Vehicle"));
	TArray<UActorComponent*> Components = GetComponentsByTag(UActorComponent::StaticClass(), TagName);

	for (UActorComponent* Component : Components)
	{
		USkeletalMeshComponent* SkelComponent = Cast<USkeletalMeshComponent>(Component);
		if (IsValid(SkelComponent))
		{
			SkelComponent->SetVisibility(bOnRiding, true);
		}
	}
}

void ADW_CharacterBase::PlayVehicleMontage(UAnimMontage* Montage)
{
	FName TagName(TEXT("Vehicle"));
	TArray<UActorComponent*> Components = GetComponentsByTag(UActorComponent::StaticClass(), TagName);

	for (UActorComponent* Component : Components)
	{
		USkeletalMeshComponent* SkelComponent = Cast<USkeletalMeshComponent>(Component);
		if (IsValid(SkelComponent))
		{
			SkelComponent->GetAnimInstance()->Montage_Play(Montage);
		}
	}
}

AActor* ADW_CharacterBase::FindBestLockOnTarget()
{
	TArray<AActor*> Candidates;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADW_MonsterBase::StaticClass(), Candidates);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return nullptr;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	FVector2D ScreenCenter = ViewportSize * 0.5f;

	AActor* BestTarget = nullptr;
	float ClosestDistSquared = FLT_MAX;

	const float MaxLockOnDistance = 5000.f;

	for (AActor* Candidate : Candidates)
	{
		if (!IsValid(Candidate) || Candidate == this) continue;
		if (!PC->LineOfSightTo(Candidate)) continue;

		float Distance = FVector::Dist(GetActorLocation(), Candidate->GetActorLocation());
		if (Distance > MaxLockOnDistance) continue;

		FVector2D ScreenPos;
		bool bOnScreen = PC->ProjectWorldLocationToScreen(Candidate->GetActorLocation(), ScreenPos);

		if (bOnScreen)
		{
			float DistSq = FVector2D::DistSquared(ScreenPos, ScreenCenter);
			if (DistSq < ClosestDistSquared)
			{
				ClosestDistSquared = DistSq;
				BestTarget = Candidate;
			}
		}
	}

	return BestTarget;
}

void ADW_CharacterBase::UpdateLockOnRotation()
{
	if (!bIsLockOn)
	{
		GetWorldTimerManager().ClearTimer(LockOnRotationTimer);
		LockOnTarget = nullptr;
		return;
	}
	
	ADW_MonsterBase* MonsterTarget = Cast<ADW_MonsterBase>(LockOnTarget);
	if (!IsValid(LockOnTarget) ||
	!GetController()->LineOfSightTo(LockOnTarget) ||
	(MonsterTarget && MonsterTarget->bIsDead))
	{
		AActor* NewTarget = FindClosestTarget(800.f);

		if (IsValid(NewTarget) && NewTarget != LockOnTarget)
		{
			LockOnTarget = NewTarget;
		}
		else
		{
			ToggleLockOn();
			return;
		}
	}

	// 회전 처리
	FVector ToTarget = LockOnTarget->GetActorLocation() - GetActorLocation();
	FRotator DesiredRotation = ToTarget.Rotation();

	float HeightDiff = ToTarget.Z;
	float MaxHeightEffect = 200.f;
	float TargetPitch = FMath::Clamp(HeightDiff / MaxHeightEffect, -1.f, 1.f) * 30.f;

	DesiredRotation.Pitch = TargetPitch;
	DesiredRotation.Roll = 0.f;

	FRotator InterpRot = FMath::RInterpTo(
		GetControlRotation(),
		DesiredRotation,
		GetWorld()->GetDeltaSeconds(),
		10.f
	);

	GetController()->SetControlRotation(InterpRot);
}

void ADW_CharacterBase::UpdateLockOnCandidates()
{
	LockOnCandidates.Empty();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	TArray<AActor*> AllTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADW_MonsterBase::StaticClass(), AllTargets);

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	FVector2D ScreenCenter = ViewportSize * 0.5f;

	for (AActor* Target : AllTargets)
	{
		if (!IsValid(Target) || Target == this) continue;
		if (!PC->LineOfSightTo(Target)) continue;

		FVector2D ScreenPos;
		if (PC->ProjectWorldLocationToScreen(Target->GetActorLocation(), ScreenPos))
		{
			if (ScreenPos.X > ScreenCenter.X)
			{
				LockOnCandidates.Add(Target);
			}
		}
	}

	// 화면 중심 가까운 순 정렬
	LockOnCandidates.Sort([&](AActor& A, AActor& B)
	{
		FVector2D APos, BPos;
		PC->ProjectWorldLocationToScreen(A.GetActorLocation(), APos);
		PC->ProjectWorldLocationToScreen(B.GetActorLocation(), BPos);
		return FVector2D::DistSquared(APos, ScreenCenter) < FVector2D::DistSquared(BPos, ScreenCenter);
	});
}

void ADW_CharacterBase::UpdateLockOnMarkerPosition()
{
	if (!bIsLockOn || !IsValid(LockOnTarget) || !IsValid(LockOnWidgetInstance)) return;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FVector WorldLocation;

	// 메시 소켓 기준으로 락온 마커 위치 지정
	USkeletalMeshComponent* TargetMesh = LockOnTarget->FindComponentByClass<USkeletalMeshComponent>();
	
	if (TargetMesh->DoesSocketExist("spine_02"))
	{
		WorldLocation = TargetMesh->GetSocketLocation("spine_02");
	}
	else if (TargetMesh->DoesSocketExist("spine2"))
	{
		WorldLocation = TargetMesh->GetSocketLocation("spine2");
	}
	else
	{
		// 예외 상황엔 캡슐 기준으로
		UCapsuleComponent* Capsule = LockOnTarget->FindComponentByClass<UCapsuleComponent>();
		if (Capsule)
		{
			WorldLocation = LockOnTarget->GetActorLocation() + FVector(0.f, 0.f, Capsule->GetScaledCapsuleHalfHeight() * 0.6f);
		}
		else
		{
			WorldLocation = LockOnTarget->GetActorLocation();
		}
	}

	// 화면에 락온 마커 표시
	FVector2D ScreenPosition;
	if (PC->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition))
	{
		LockOnWidgetInstance->SetPositionInViewport(ScreenPosition, true);
	}

	if (ULockOnWidget* LockOnWidget = Cast<ULockOnWidget>(LockOnWidgetInstance))
	{
		float Distance = FVector::Dist(GetActorLocation(), LockOnTarget->GetActorLocation());
		LockOnWidget->SetMarkerSizeByDistance(Distance);
	}
}

void ADW_CharacterBase::SwitchLockOnTarget()
{
	if (!bIsLockOn) return;

	UpdateLockOnCandidates();

	if (LockOnCandidates.Num() == 0) return;

	LockOnIndex = (LockOnIndex + 1) % LockOnCandidates.Num();
	LockOnTarget = LockOnCandidates[LockOnIndex];
}


void ADW_CharacterBase::UpdateFootstepSurface()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 100.f);  // 아래 방향으로 트레이스

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		CurrentSurfaceType = UGameplayStatics::GetSurfaceType(Hit);
	}
}

void ADW_CharacterBase::SpawnFootstepEffect(const FName FootSocketName) const
{
	if (bIsRidingVehicle) return;
	
	const FVector NewFootLocation = GetMesh()->GetSocketLocation(FootSocketName);
	const FVector NewTraceStart = NewFootLocation + FVector(0, 0, 100);
	const FVector NewTraceEnd = NewFootLocation - FVector(0, 0, 500);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetMesh()->GetOwner());
	Params.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(Hit, NewTraceStart, NewTraceEnd, ECC_Visibility, Params))
	{
		if (UNiagaraSystem* const* FoundSystem = FootstepVFXMap.Find(CurrentSurfaceType))
		{
			FVector FootLocation = Hit.ImpactPoint;

			if (GetMesh()->DoesSocketExist(FootSocketName))
			{
				FootLocation = GetMesh()->GetSocketLocation(FootSocketName);
			}

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), *FoundSystem, FootLocation, Hit.ImpactNormal.Rotation());
		}
	}
}

void ADW_CharacterBase::SpawnFootstepEffect_H(const FName FootSocketName) const
{
	if (!bIsRidingVehicle) return;
	
	const FVector NewFootLocation = Vehicle->GetSocketLocation(FootSocketName);
	const FVector NewTraceStart = NewFootLocation + FVector(0, 0, 100);
	const FVector NewTraceEnd = NewFootLocation - FVector(0, 0, 500);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetMesh()->GetOwner());
	Params.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(Hit, NewTraceStart, NewTraceEnd, ECC_Visibility, Params))
	{
		if (UNiagaraSystem* const* FoundSystem = FootstepVFXMap.Find(CurrentSurfaceType))
		{
			FVector FootLocation = Hit.ImpactPoint;

			if (Vehicle->DoesSocketExist(FootSocketName))
			{
				FootLocation = Vehicle->GetSocketLocation(FootSocketName);
			}

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), *FoundSystem, FootLocation, Hit.ImpactNormal.Rotation());
		}
	}
}