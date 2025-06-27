// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DW_SkillData.h"
#include "DW_SkillManager.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDW_SkillManager : public UObject
{
	GENERATED_BODY()
public:
    void Initialize(UDataTable* InSkillDataTable);

    const FSkillData* GetSkillData(FName SkillID) const;

    /** 선행 조건이 충족됐는지 확인 */
    bool CanUnlockSkill(FName SkillID, const TMap<FName, FSkillState>& SkillStateMap) const;

    /** 스킬 배우기*/
    UFUNCTION(BlueprintCallable)
    bool TryLearnSkill(FName SkillID, int32& InOutSP, TMap<FName, FSkillState>& SkillStateMap, UDW_AttributeComponent* AttrComp);

    /** 스킬 레벨 가져오기 */
    void ApplySkillEffect(const FSkillData& SkillData, int32 DeltaLevel, UDW_AttributeComponent* AttrComp);


private:
    UPROPERTY()
    UDataTable* SkillDataTable;
};
