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

    // 현재 SP (스킬 포인트)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 CurrentSP = 5;

    // 전체 스킬 데이터 (데이터 테이블 참조)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    UDataTable* SkillDataTable;

    // 현재 플레이어의 스킬 상태들
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
    TMap<FName, FSkillState> SkillStateMap;

    // 스킬을 배우려 시도
    UFUNCTION(BlueprintCallable)
    bool TryLearnSkill(FName SkillID);

    // 현재 스킬 레벨 확인
    UFUNCTION(BlueprintCallable)
    int32 GetSkillLevel(FName SkillID) const;

    // SP 변경 시 브로드캐스트
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillUpdated);
    UPROPERTY(BlueprintAssignable)
    FOnSkillUpdated OnSkillUpdated;
};