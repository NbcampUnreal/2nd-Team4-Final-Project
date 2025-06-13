// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_InteractInterface.h"
#include "GameFramework/Actor.h"
#include "DW_Chest.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Chest : public AActor, public IDW_InteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADW_Chest();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bHasOpened = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsChest = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", meta = (EditCondition = "bIsChest", EditConditionHides))
	TSubclassOf<class AWorldItemActor> WorldItemActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TSubclassOf<class ADW_MonsterBase> Mimic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", meta = (EditCondition = "bIsChest", EditConditionHides))
	int32 ItemCode = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* ChestMontage;
	
	virtual void Interact_Implementation(AActor* Interactor) override;

	void SpawnMimic();

	void DropItem();
};
