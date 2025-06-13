// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_InteractItemBase.h"
#include "DW_MimicChest.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UAnimMontage;

UCLASS()
class DW_DALWONDER_API ADW_MimicChest : public ADW_InteractItemBase
{
	GENERATED_BODY()
	
public:	
	ADW_MimicChest();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterSetting", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterSetting")
    USkeletalMeshComponent* SkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterSetting")
    UAnimMontage* OpenChestMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterSetting")
    bool bIsMonster = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterSetting", meta = (EditCondition = "bIsMonster"))
    TSubclassOf<AActor> MonsterClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterSetting", meta = (EditCondition = "!bIsMonster"))
    int32 ItemCode;

    bool bHasOpened = false;

public:
    virtual void Interact_Implementation(AActor* Interactor) override;

protected:
    void SpawnMonster();
    void DropItem();
    void PlayOpenAnimation();
};