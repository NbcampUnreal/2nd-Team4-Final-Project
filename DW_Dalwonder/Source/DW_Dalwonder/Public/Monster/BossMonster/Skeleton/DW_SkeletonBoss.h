// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BossMonster/DW_BossMonsterBase.h"
#include "DW_SkeletonBoss.generated.h"


UENUM(BlueprintType)
enum class ESkeletonFormType : uint8
{
	Warrior UMETA(DisplayName = "Warrior"),
	Bandit UMETA(DisplayName = "Bandit"),
	Archer UMETA(DisplayName = "Archer"),
	Mage UMETA(DisplayName = "Mage"),
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FFormData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* RightHandWeapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* LeftHandWeapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> Montages;
};

UCLASS()
class DW_DALWONDER_API ADW_SkeletonBoss : public ADW_BossMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_SkeletonBoss();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* LeftHandWeaponComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* RightHandWeaponComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	ESkeletonFormType CurrentFormType = ESkeletonFormType::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FormData")
	TMap<ESkeletonFormType, FFormData> FormDataMap;

	UFUNCTION(BlueprintCallable)
	void ChangeFormTypeByRandom();

	UFUNCTION(BlueprintCallable)
	void ChangeFormType(const ESkeletonFormType FormType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
