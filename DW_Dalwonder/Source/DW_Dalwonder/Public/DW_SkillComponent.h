// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "DW_SkillData.h"
#include "DW_SkillState.h"
#include "DW_SkillComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DW_DALWONDER_API UDW_SkillComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
    UDW_SkillComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 CurrentSP = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    UDataTable* SkillDataTable;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
    TMap<FName, FSkillState> SkillStateMap;

    UFUNCTION(BlueprintCallable)
    bool TryLearnSkill(FName SkillID);

    UFUNCTION(BlueprintCallable)
    int32 GetSkillLevel(FName SkillID) const;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillUpdated);
    UPROPERTY(BlueprintAssignable)
    FOnSkillUpdated OnSkillUpdated;
};