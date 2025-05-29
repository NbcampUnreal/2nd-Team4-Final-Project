#include "Character/DW_CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "Character/DW_PlayerController.h"
#include "Character/CharacterStatComponent.h"
#include "Monster/DW_MonsterBase.h"
#include "DW_DamageType.h"
#include "DW_GmBase.h"
#include "Item/WorldItemActor.h"
#include "Item/ItemDataManager.h"
#include "UI/Widget/HUDWidget.h"
#include "DW_InteractInterface.h"
#include "Engine/DamageEvents.h"


ADW_CharacterBase::ADW_CharacterBase()
{
	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("StatComponent"));
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->SocketOffset = FVector(0.f, 50.f, 50.f);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetWalkSpeed();;
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ADW_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer  //아이템 업그레이드 타이머
	(
		ItemScanTimerHandle,
		this,
		&ADW_CharacterBase::UpdateClosestItem,
		0.1f,         
		true          
	);

	InventoryComponent->InitializeSlots();	// 인벤토리 슬롯 초기화

	// HUD 타이머 설정 (0.1초 간격)
	GetWorld()->GetTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ADW_CharacterBase::UpdateHUD,
		0.1f,
		true  // 반복 여부
	);
}

void ADW_CharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(AttackTimer);
	AttackTimer.Invalidate();
	GetWorldTimerManager().ClearTimer(BlockTimer);
	BlockTimer.Invalidate();
	GetWorldTimerManager().ClearTimer(IdleStateTimer);
	IdleStateTimer.Invalidate();
}

void ADW_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
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

			if (PlayerController->SprintAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Started,
					this,
					&ADW_CharacterBase::Sprint);
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

			if (PlayerController->InteractAction)
			{
				UE_LOG(LogTemp, Warning, TEXT("[입력 바인딩] InteractAction 바인딩 시작"));

				EnhancedInputComponent->BindAction(
					PlayerController->InteractAction,
					ETriggerEvent::Started,
					this,
					&ADW_CharacterBase::Interact);

				UE_LOG(LogTemp, Warning, TEXT("[입력 바인딩] InteractAction 바인딩 완료"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[입력 바인딩] InteractAction이 nullptr임!"));
			}

			if (PlayerController->ESCAction)
			{
				// ESC 바인딩
				EnhancedInputComponent->BindAction(
					PlayerController->ESCAction,
					ETriggerEvent::Started,
					this,
					&ADW_CharacterBase::ToggleESCMenu
				);
			}
		}
	}
}

void ADW_CharacterBase::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	if (!bCanControl) return;

	FVector2D MoveInput = Value.Get<FVector2D>();
	FRotator ControlRotation = Controller->GetControlRotation();
	FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
	
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardVector, MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightVector, MoveInput.Y);
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
	if (Value.Get<bool>())
	{
		StartAttack();
	}
}

void ADW_CharacterBase::Sprint(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		if (bIsSprinting == false)
		{
			bIsSprinting = true;
			GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetSprintSpeed();
			GetCharacterStatComponent()->ConsumeStamina(1.f);
		}
		else
		{
			bIsSprinting = false;
			GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetWalkSpeed();
			GetCharacterStatComponent()->StopConsumeStamina();
		}
	}
}

void ADW_CharacterBase::Dodge(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		if (GetCharacterStatComponent()->GetStamina() < 10.f)
		{
			return;
		}
		
		//@TODO : Dodge 로직 구현
		GetCharacterStatComponent()->SetStamina(GetCharacterStatComponent()->GetStamina() - 10.f);
		PlayMontage(DodgeMontage);
	}
}

void ADW_CharacterBase::Lockon(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		/*@TODO : Lockon 함수 구현
		 *bIsLockon 변수로 스위치하면 됨*/
	}
}

void ADW_CharacterBase::PlayMontage(UAnimMontage* Montage, bool bBlockControl, int32 SectionIndex)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		if (!IsValid(Montage) || SectionIndex >= Montage->GetNumSections()) return;
		
		if (SectionIndex != -1)
		{
			FName SectionName = Montage->GetSectionName(SectionIndex);
			AnimInstance->Montage_Play(Montage);
			AnimInstance->Montage_JumpToSection(SectionName);

			if (bBlockControl == true)
			{
				float SectionLength = Montage->GetSectionLength(SectionIndex);
				BlockCharacterControl(true, SectionLength);
			}
		}
		else
		{
			AnimInstance->Montage_Play(Montage, 1.f, EMontagePlayReturnType::MontageLength, 0, true);

			if (bBlockControl == true)
			{
				float MontageLength = Montage->GetPlayLength();
				BlockCharacterControl(true, MontageLength);
			}
		}
	}
}

void ADW_CharacterBase::SetCombatState(ECharacterCombatState NewState)
{
	CurrentCombatState = NewState;
	UE_LOG(LogTemp, Log, TEXT("전투 상태 변경: %s"), *UEnum::GetValueAsString(NewState));

	if (CurrentCombatState != ECharacterCombatState::Idle || CurrentCombatState != ECharacterCombatState::Guarding)
	{
		SetIdleState();
	}
}

void ADW_CharacterBase::StartAttack()
{
	if (!bCanAttack) return;
	
	SetCombatState(ECharacterCombatState::Attacking);
	bCanControl = false;

	if (GetMovementComponent()->IsFalling())
	{
		UE_LOG(LogTemp, Warning, TEXT("Falling"));
		check(IsValid(FallingAttackMontage));
		bCanAttack = false;
		PlayMontage(FallingAttackMontage, true);
		SetAttackTimer(FallingAttackMontage);
	}
	else if (bIsGuarding)
	{
		UE_LOG(LogTemp, Warning, TEXT("Guard"));
		check(IsValid(GuardAttackMontage));
		bCanAttack = false;
		PlayMontage(GuardAttackMontage, true);
		SetAttackTimer(GuardAttackMontage);
		SetGuarding(false);
	}
	else if (bIsSprinting && GetCharacterMovement()->Velocity.Length() > GetCharacterStatComponent()->GetWalkSpeed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint"));
		check(IsValid(SprintAttackMontage));
		bCanAttack = false;
		PlayMontage(SprintAttackMontage, true);
		SetAttackTimer(SprintAttackMontage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("else"));
		check(IsValid(AttackMontage));
		
		int ComboTotalNum = AttackMontage->GetNumSections();
		
		if (bCanAttack && ComboIndex < ComboTotalNum)
		{
			bCanAttack = false;
			PlayMontage(AttackMontage, true, ComboIndex);
			SetAttackTimer(AttackMontage, ComboIndex);
			ComboIndex++;
		}
	}
}

void ADW_CharacterBase::CancelAttack()
{
	if (CurrentCombatState == ECharacterCombatState::Attacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("공격 취소"));
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 현재 재생 중인 모든 몽타주를 중단
			AnimInstance->Montage_Stop(0.2f);
		}
		
		// 타이머도 정리
		GetWorldTimerManager().ClearTimer(AttackTimer);
		// 기존 공격 종료 처리
		EndAttack(nullptr, true);
		BlockCharacterControl(false);

		// 튕김 애니메이션 재생
		if (IsValid(DodgeMontage))
		{
			PlayMontage(DodgeMontage);
		}
	}
}

void ADW_CharacterBase::EndAttack(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("EndAttack Called!!"));

	SetCombatState(ECharacterCombatState::Idle);
	bCanAttack = true;
	ComboIndex = 0;
}

float ADW_CharacterBase::TakeDamage
	(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser
	)
{
	ADW_MonsterBase* Monster = Cast<ADW_MonsterBase>(DamageCauser);

	// 캐릭터가 무적 상태일 때
	if (bIsInvincible)
	{
		UE_LOG(LogTemp, Log, TEXT("무적"));
		return 0.f;
	}

	// 캐릭터가 몬스터에게 대미지를 받았을 때
	if (IsValid(Monster))
	{
		// 몬스터가 패링 가능한 상태이고, 캐릭터의 State가 Parrying일 때
		if (Monster->GetCanParry() && CurrentCombatState == ECharacterCombatState::Parrying)
		{
			Monster->Parried();
			PlayMontage(ParryMontage, true);
		}
		else if (CurrentCombatState == ECharacterCombatState::Guarding)
		{
			
		}
		else
		{
			UDW_DamageType* DW_DamageType = Cast<UDW_DamageType>(DamageEvent.DamageTypeClass);

			// 대미지 타입을 커스텀 대미지 타입으로 받았을 때
			if (IsValid(DW_DamageType))
			{
				// 넉백이 적용되는 공격을 맞았을 때
				if (DW_DamageType->bCanKnockdown == true)
				{
					KnockBackCharacter();
				}
				// 넉백이 적용되지 않는 공격을 맞았을 때
				else
				{
					float KnockBackAmount = GetCharacterStatComponent()->GetMaxHealth() * 0.3f;
					if (DamageAmount > KnockBackAmount)
					{
						KnockBackCharacter();
					}
					else
					{
						int32 HitSectionNum = HitMontage->GetNumSections();
						int32 RandomHitSectionNum = FMath::RandRange(0, HitSectionNum - 1);
						PlayMontage(HitMontage, true, RandomHitSectionNum);
					}
				}
			}
			// 일반 대미지 타입으로 받았을 때
			else
			{
				int32 HitSectionNum = HitMontage->GetNumSections();
				int32 RandomHitSectionNum = FMath::RandRange(0, HitSectionNum - 1);
				PlayMontage(HitMontage, true, RandomHitSectionNum);
			}

			// 체력 감소 로직
			StatComponent->SetHealth(StatComponent->GetHealth() - DamageAmount);
		}
	}

	if (FMath::IsNearlyZero(StatComponent->GetHealth()))
	{
		Dead();
	}
	
	return DamageAmount;
}

void ADW_CharacterBase::SetParrying(bool bNewParrying)
{
	if (bIsParrying == bNewParrying)
		return;
	
	bIsParrying = bNewParrying;
	
	if (bIsParrying)
	{
		SetCombatState(ECharacterCombatState::Parrying);
		UE_LOG(LogTemp, Log, TEXT("패링 시작"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("패링 종료"));
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
		SetCombatState(ECharacterCombatState::Guarding);
		UE_LOG(LogTemp, Log, TEXT("가드 시작"));
		GetCharacterStatComponent()->ConsumeStamina(2.f);
	}
	else
	{
		SetCombatState(ECharacterCombatState::Idle);
		UE_LOG(LogTemp, Log, TEXT("가드 종료"));
		GetCharacterStatComponent()->StopConsumeStamina();
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
	if (GetCharacterStatComponent()->GetStamina() < 5.f)
	{
		return;
	}
	
	SetGuarding(true);
	BlockCharacterControl(true);
	PlayMontage(GuardMontage);
}

void ADW_CharacterBase::EndGuard()
{
	SetGuarding(false);
	PlayMontage(GuardMontage, true, 2);
}

void ADW_CharacterBase::KnockBackCharacter()
{
	const float KnockBackMultiplier = 50.f;
	const FVector KnockBackDirection = -GetActorForwardVector() * KnockBackMultiplier;
	
	LaunchCharacter(KnockBackDirection, true, true);
	if (IsValid(KnockBackMontage) == true)
	{
		PlayMontage(KnockBackMontage, true);
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

void ADW_CharacterBase::Dead()
{
	if (CurrentCombatState == ECharacterCombatState::Attacking)
	{
		PlayMontage(DeadMontage, true, 1);
	}
	else
	{
		PlayMontage(DeadMontage, true);
	}
}

void ADW_CharacterBase::SetAttackTimer(UAnimMontage* Montage, int32 SectionIndex)
{
	float MontageLength;
	
	if (SectionIndex == -1)
	{
		MontageLength = Montage->GetPlayLength();
	}
	else
	{
		MontageLength = Montage->GetSectionLength(SectionIndex);
	}
	
	check(IsValid(GetWorld()));
	
	GetWorldTimerManager().SetTimer(AttackTimer, FTimerDelegate::CreateLambda([&]()
		{
			EndAttack(Montage, false);
		}), MontageLength, false);
}

void ADW_CharacterBase::SetIdleState()
{
	GetWorldTimerManager().ClearTimer(IdleStateTimer);

	GetWorldTimerManager().SetTimer(IdleStateTimer, FTimerDelegate::CreateLambda([&]
	{
			SetCombatState(ECharacterCombatState::Idle);
	}), 5.f, false);
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

	DrawDebugSphere(GetWorld(), End, realSphereRadius, 12, FColor::Green, false, 1.0f);

	if (bHit && Hit.bBlockingHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<UDW_InteractInterface>())
		{
			UE_LOG(LogTemp, Warning, TEXT("[Interact] 맞은 액터: %s"), *HitActor->GetName());
			IDW_InteractInterface::Execute_Interact(HitActor, this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[Interact] 인터페이스 미구현 액터: %s"), *GetNameSafe(HitActor));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Interact] 아무것도 맞지 않음."));
	}

	if (CurrentItem)
	{

		FItemData Data = CurrentItem->GetItemData(); // 아이템 정보 가져오기
		bool bAdded = InventoryComponent->AddItem(Data);
		UItemDataManager* ItemDataManager = UItemDataManager::GetInstance();
		if (ItemDataManager)
		{
			bool bSuccess;
			FName TargetItemID = Data.ItemID; // 데이터테이블에 있는 ItemID

			FItemData BaseData = ItemDataManager->GetItemBaseData(TargetItemID, bSuccess);
			if (bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("Item Found: %s (Type: %s)"), *BaseData.ItemName.ToString(), *UEnum::GetValueAsString(BaseData.ItemType));

				if (BaseData.ItemType == EItemType::Equipment)
				{
					FEquipmentSubData EquipData = ItemDataManager->GetSubData<FEquipmentSubData>(TargetItemID, bSuccess);
					if (bSuccess)
					{
						UE_LOG(LogTemp, Log, TEXT("Equipment Data: Damage=%.1f, Slot=%s"), EquipData.AttackDamage, *UEnum::GetValueAsString(EquipData.EquipmentSlot));
					}
				}
				if (BaseData.ItemType == EItemType::Consumable)
				{
					FConsumableSubData ConsumData = ItemDataManager->GetSubData<FConsumableSubData>(TargetItemID, bSuccess);
					if (bSuccess)
					{
						UE_LOG(LogTemp, Log, TEXT("Consume Data: HealAmount=%.1f, ManaRestoreAmount=%.1f"), ConsumData.HealAmount, ConsumData.ManaRestoreAmount);
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Item ID '%s' not found in ItemDataManager."), *TargetItemID.ToString());
			}
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, bAdded ? FColor::Green : FColor::Red,
				FString::Printf(TEXT("Item %s %s"),
					*Data.ItemName.ToString(),
					bAdded ? TEXT("added to inventory!") : TEXT("failed to add!")
				));
		}

		if (bAdded)
		{
			CurrentItem->Destroy();
			CurrentItem = nullptr;
		}

		if (ADW_PlayerController* PC = Cast<ADW_PlayerController>(GetController()))
		{
			PC->RequestInventoryUIUpdate();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("아이템 없음"));
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
			HUD->UpdateHPBar(StatComponent->GetHealth(), StatComponent->GetMaxHealth());
			HUD->UpdateStaminaBar(StatComponent->GetStamina(), StatComponent->GetMaxStamina());
		}
		else {
			//캐스팅 실패시 타이머 초기화
			GetWorld()->GetTimerManager().ClearTimer(HUDUpdateTimerHandle);
		}
		//현재 HP, Stemina만 업데이트중 아이템(물약) 사용시도 필요하면 제작
	}
}

void ADW_CharacterBase::ToggleESCMenu()
{
	ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
	if (!GameMode || !ESCMenuWidgetClass) return;

	if (!bIsESCMenuOpen)
	{
		ESCMenuWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), ESCMenuWidgetClass);
		if (ESCMenuWidgetInstance)
		{
			GameMode->SwitchUI(ESCMenuWidgetClass);  // ESC 메뉴 열기
			bIsESCMenuOpen = true;

			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				PC->SetShowMouseCursor(true);
				// UI Focus말고 키보드 입력도 먹도록 수정
				FInputModeGameAndUI InputMode;
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				InputMode.SetHideCursorDuringCapture(false);
				PC->SetInputMode(InputMode);
			}
		}
	}
	else
	{
		if (ESCMenuWidgetInstance)
		{
			GameMode->ClosePopupUI(ESCMenuWidgetInstance);  // ESC 메뉴 닫기
			ESCMenuWidgetInstance = nullptr;
		}

		bIsESCMenuOpen = false;

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}

void ADW_CharacterBase::ToggleLockOn()
{
	if (bIsLockOn)
	{
		// 🔓 락온 해제
		bIsLockOn = false;
		LockOnTarget = nullptr;
		GetWorldTimerManager().ClearTimer(LockOnRotationTimer);
	}
	else
	{
		// 🔒 락온 시작
		AActor* Target = FindBestLockOnTarget(); // 또는 FindClosestTarget();
		if (IsValid(Target))
		{
			bIsLockOn = true;
			LockOnTarget = Target;

			GetWorldTimerManager().SetTimer(
				LockOnRotationTimer,
				this,
				&ADW_CharacterBase::UpdateLockOnRotation,
				0.05f,
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

		// 🔍 LineOfSight 검사 (시야 안에 있는지)
		if (IsValid(PC) && !PC->LineOfSightTo(Monster)) continue;

		ClosestDistance = Distance;
		ClosestTarget = Monster;
	}

	return ClosestTarget;
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

	for (AActor* Candidate : Candidates)
	{
		if (!IsValid(Candidate) || Candidate == this) continue;
		if (!PC->LineOfSightTo(Candidate)) continue;

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
	if (!bIsLockOn || !IsValid(LockOnTarget))
	{
		GetWorldTimerManager().ClearTimer(LockOnRotationTimer);
		bIsLockOn = false;
		LockOnTarget = nullptr;
		return;
	}

	FVector ToTarget = LockOnTarget->GetActorLocation() - GetActorLocation();
	FRotator DesiredRotation = ToTarget.Rotation();
	DesiredRotation.Pitch = 0.f;
	DesiredRotation.Roll = 0.f;

	// 🔁 Controller 회전 → 캐릭터 & 카메라 모두 회전
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
			// 오른쪽에 있는 타겟만 선별 (왼쪽 정렬 원하면 반대로)
			if (ScreenPos.X > ScreenCenter.X)
			{
				LockOnCandidates.Add(Target);
			}
		}
	}

	// 👉 화면 중심 가까운 순 정렬
	LockOnCandidates.Sort([&](AActor& A, AActor& B)
	{
		FVector2D APos, BPos;
		PC->ProjectWorldLocationToScreen(A.GetActorLocation(), APos);
		PC->ProjectWorldLocationToScreen(B.GetActorLocation(), BPos);
		return FVector2D::DistSquared(APos, ScreenCenter) < FVector2D::DistSquared(BPos, ScreenCenter);
	});
}

void ADW_CharacterBase::SwitchLockOnTarget()
{
	if (!bIsLockOn) return;

	UpdateLockOnCandidates();

	if (LockOnCandidates.Num() == 0) return;

	LockOnIndex = (LockOnIndex + 1) % LockOnCandidates.Num();
	LockOnTarget = LockOnCandidates[LockOnIndex];
}
