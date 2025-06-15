// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BossMonster/DW_BossMonsterBase.h"
#include "DW_SkeletonBoss.generated.h"

// 상태는 EnumType의 Index로 관리함. 모든 배열의 0번부터 Warrior, Bandit, Archer, Mage로 해야 함.

UENUM(BlueprintType)
enum class ESkeletonFormType : uint8
{
	Warrior UMETA(DisplayName = "Warrior"),
	Bandit UMETA(DisplayName = "Bandit"),
	Archer UMETA(DisplayName = "Archer"),
	Mage UMETA(DisplayName = "Mage"),
	Max UMETA(Hidden)
};

UCLASS()
class DW_DALWONDER_API ADW_SkeletonBoss : public ADW_BossMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_SkeletonBoss();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* LeftHandWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* RightHandWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TArray<USkeletalMeshComponent*> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TArray<USkeletalMesh*> SkeletonMeshs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TArray<TSubclassOf<UAnimInstance>> SkeletonABP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> WarriorMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> BanditMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> ArcherMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> MageMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> CurrentMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ESkeletonFormType CurrentFormType = ESkeletonFormType::Warrior;

	ESkeletonFormType GetRandomFormType();

	UFUNCTION(BlueprintCallable)
	void ChangeFormType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
