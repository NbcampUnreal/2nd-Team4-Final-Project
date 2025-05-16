#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacterCombatState.h"
#include "GameFramework/Character.h"
#include "DW_CharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;

// ✅ 캐릭터의 기본 클래스: 이동, 전투, 입력 처리 등 공통 기능 포함
UCLASS()
class DW_DALWONDER_API ADW_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADW_CharacterBase();

	virtual void Tick(float DeltaTime) override;

protected:
	// ▶ 게임 시작 시 초기 설정 (예: 상태 초기화)
	virtual void BeginPlay() override;

public:
	// ▶ 플레이어 입력 바인딩 (InputAction → 함수 연결)
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// -----------------------------
	// 📌 입력 처리 관련 함수
	// -----------------------------
	UFUNCTION()
	void Move(const FInputActionValue& Value);             // 이동 입력

	UFUNCTION()
	void Look(const FInputActionValue& Value);             // 마우스/패드 시점 회전 입력

	UFUNCTION()
	void StartJump(const FInputActionValue& Value);        // 점프 시작

	UFUNCTION()
	void StopJump(const FInputActionValue& Value);         // 점프 중지

	// -----------------------------
	// 📌 데미지 처리 함수
	// -----------------------------
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	// -----------------------------
	// 📌 카메라 및 무기 관련 컴포넌트
	// -----------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;                        // 카메라 붐(캐릭터 뒤에 부착)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;                              // 실제 시점 카메라

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* Weapon;                          // 캐릭터의 무기 액터

	bool bCanControl = true;                               // 캐릭터 조작 가능 여부

	// -----------------------------
	// ⚔️ 전투 관련 시스템 (Combat)
	// -----------------------------
#pragma region Combat

public:
	// 전투 상태 변경 (예: Idle → Attacking)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCombatState(ECharacterCombatState NewState);

	// 공격 애니메이션 재생
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PlayAttackMontage();

	// 패링 상태 설정
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetParrying(bool bNewParrying);

	// 가드 상태 설정
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetGuarding(bool bNewParrying);

	// 무적 상태 설정
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetInvincible(bool bNewParrying);

	// 가드 시작
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartGuard();

	// 가드 종료
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndGuard();

	// 캐릭터 넉백 처리 (피격 반응)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void KnockBackCharacter();

	// 조작 차단 여부 설정 (피격 중 무력화 등)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void BlockCharacterControl(bool bShouldBlock);

	// 현재 전투 상태 (Idle, Attacking 등)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	ECharacterCombatState CurrentCombatState = ECharacterCombatState::Idle;

	// 공격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	// 넉백 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* KnockBackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* GuardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* ParryMontage;

protected:
	// 패링 중 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsParrying = false;

	// 가드 중 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsGuarding = false;

	// 무적 상태 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsInvincible = false;

#pragma endregion

#pragma region Interact
	// -----------------------------
	//  상호작용 관련
	// -----------------------------
public:
	
	FTimerHandle ItemScanTimerHandle;
	
	UFUNCTION()
	void Interact();
	void AddNearbyItem(class AWorldItemActor* Item);
	void RemoveNearbyItem(AWorldItemActor* Item);
	void UpdateClosestItem();

protected:	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	AWorldItemActor* CurrentItem = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TArray<AWorldItemActor*> NearbyItems;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float SphereRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY()
	UUserWidget* InteractionWidget;

private:
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractDistance = 300.f;

	AActor* CurrentInteractTarget = nullptr;

#pragma endregion
};
