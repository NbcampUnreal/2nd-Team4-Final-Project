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

    // ���� SP (��ų ����Ʈ)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 CurrentSP = 5;

    // ��ü ��ų ������ (������ ���̺� ����)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    UDataTable* SkillDataTable;

    // ���� �÷��̾��� ��ų ���µ�
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
    TMap<FName, FSkillState> SkillStateMap;

    // ��ų�� ���� �õ�
    UFUNCTION(BlueprintCallable)
    bool TryLearnSkill(FName SkillID);

    // ���� ��ų ���� Ȯ��
    UFUNCTION(BlueprintCallable)
    int32 GetSkillLevel(FName SkillID) const;

    // SP ���� �� ��ε�ĳ��Ʈ
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillUpdated);
    UPROPERTY(BlueprintAssignable)
    FOnSkillUpdated OnSkillUpdated;
};