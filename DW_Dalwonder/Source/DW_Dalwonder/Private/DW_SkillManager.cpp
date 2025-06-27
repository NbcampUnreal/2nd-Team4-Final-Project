// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_SkillManager.h"
#include "DW_AttributeComponent.h"
#include "DW_SkillState.h"
#include "Engine/DataTable.h"

void UDW_SkillManager::Initialize(UDataTable* InSkillDataTable)
{
    SkillDataTable = InSkillDataTable;
}

const FSkillData* UDW_SkillManager::GetSkillData(FName SkillID) const
{
    return SkillDataTable ? SkillDataTable->FindRow<FSkillData>(SkillID, TEXT("SkillLookup")) : nullptr;
}

bool UDW_SkillManager::CanUnlockSkill(FName SkillID, const TMap<FName, FSkillState>& SkillStateMap) const
{
    const FSkillData* Data = GetSkillData(SkillID);
    if (!Data)
    {
        UE_LOG(LogTemp, Warning, TEXT("CanUnlockSkill: SkillData not found for %s"), *SkillID.ToString());
        return false;
    }

    // 선행 조건이 명시되지 않은 경우 → 항상 가능
    if (Data->PrerequisiteSkillID.IsNone())
        return true;

    FString Raw = Data->PrerequisiteSkillID.ToString().TrimStartAndEnd();

    // 예외 처리: None, Null, 빈 문자열
    if (Raw.IsEmpty() || Raw.Equals(TEXT("None"), ESearchCase::IgnoreCase) || Raw.Equals(TEXT("Null"), ESearchCase::IgnoreCase))
        return true;

    // "or" 조건 분리
    TArray<FString> Tokens;
    if (Raw.Contains(TEXT(" or ")))
        Raw.ParseIntoArray(Tokens, TEXT(" or "), true);
    else
        Tokens.Add(Raw); // 단일 조건

    // 둘 중 하나라도 만족하면 true
    for (const FString& Token : Tokens)
    {
        FName PreID(*Token.TrimStartAndEnd());
        const FSkillState* PreState = SkillStateMap.Find(PreID);
        if (PreState && PreState->CurrentLevel > 0)
            return true;
    }

    // 아무 조건도 만족하지 않으면 false
    return false;
}

bool UDW_SkillManager::TryLearnSkill(FName SkillID, int32& InOutSP, TMap<FName, FSkillState>& SkillStateMap, UDW_AttributeComponent* AttrComp)
{
    const FSkillData* SkillData = GetSkillData(SkillID);
    if (!SkillData)
    {
        UE_LOG(LogTemp, Error, TEXT(" TryLearnSkill: SkillData not found for %s"), *SkillID.ToString());
        return false;
    }

    // ────────────────
    // 선행 조건 검사
    // ────────────────
    if (!SkillData->PrerequisiteSkillID.IsNone())
    {
        FString PrereqStr = SkillData->PrerequisiteSkillID.ToString();

        TArray<FString> PrereqIDs;
        PrereqStr.ParseIntoArray(PrereqIDs, TEXT(" or "), true);

        bool bAnySatisfied = false;

        for (const FString& IDStr : PrereqIDs)
        {
            FName PreID(*IDStr.TrimStartAndEnd());
            const FSkillState* PreState = SkillStateMap.Find(PreID);
            if (PreState && PreState->CurrentLevel > 0)
            {
                bAnySatisfied = true;
                break;
            }
        }

        if (!bAnySatisfied)
        {
            UE_LOG(LogTemp, Warning, TEXT(" Prerequisite not met for %s. Requires one of: %s"),
                *SkillID.ToString(), *PrereqStr);
            return false;
        }
    }

    // ────────────────
    // SP 체크
    // ────────────────
    if (InOutSP < SkillData->Cost)
    {
        UE_LOG(LogTemp, Warning, TEXT(" Not enough SP for %s (Cost: %d, Available: %d)"),
            *SkillID.ToString(), SkillData->Cost, InOutSP);
        return false;
    }

    // ────────────────
    // 현재 상태 확인
    // ────────────────
    FSkillState* State = SkillStateMap.Find(SkillID);

    if (State)
    {
        if (State->CurrentLevel >= SkillData->MaxLevel)
        {
            UE_LOG(LogTemp, Warning, TEXT(" %s is already at max level (%d)"),
                *SkillID.ToString(), SkillData->MaxLevel);
            return false;
        }

        State->CurrentLevel++;
        UE_LOG(LogTemp, Warning, TEXT(" %s leveled up to %d"), *SkillID.ToString(), State->CurrentLevel);
    }
    else
    {
        FSkillState NewState{ SkillID, 1 };
        SkillStateMap.Add(SkillID, NewState);
        UE_LOG(LogTemp, Warning, TEXT(" %s learned for the first time"), *SkillID.ToString());
    }

    // ────────────────
    // SP 차감 & 효과 적용
    // ────────────────
    InOutSP -= SkillData->Cost;
    UE_LOG(LogTemp, Warning, TEXT("Remaining SP: %d"), InOutSP);

    ApplySkillEffect(*SkillData, 1, AttrComp);

    return true;
}

void UDW_SkillManager::ApplySkillEffect(const FSkillData& SkillData, int32 DeltaLevel, UDW_AttributeComponent* AttrComp)
{
    if (!AttrComp) return;

    const FString Prefix = SkillData.SkillID.ToString().Left(3).ToLower();
    const float RawInc = static_cast<float>(SkillData.Increase) * DeltaLevel;

    auto AddBonus = [&](float& BonusField, float BaseField)
        {
            if (SkillData.IncreaseType == 1)
                BonusField += BaseField * (RawInc / 100.f);
            else
                BonusField += RawInc;
        };

    if (Prefix == TEXT("sta")) AddBonus(AttrComp->BonusMaxStamina, AttrComp->BaseMaxStamina);
    else if (Prefix == TEXT("spe")) AddBonus(AttrComp->BonusMoveSpeed, AttrComp->BaseMoveSpeed);
    else if (Prefix == TEXT("bag")) AddBonus(AttrComp->BonusMaxCarryWeight, AttrComp->BaseMaxCarryWeight);
    else if (Prefix == TEXT("hea")) AddBonus(AttrComp->BonusMaxHealth, AttrComp->BaseMaxHealth);
    else if (Prefix == TEXT("reg")) AddBonus(AttrComp->BonusHealthRegen, AttrComp->BaseHealthRegen);
    else if (Prefix == TEXT("pro")) AddBonus(AttrComp->BonusStaminaRegen, AttrComp->BaseStaminaRegen);
    else if (Prefix == TEXT("lon")) AddBonus(AttrComp->BonusLongswordXPMod, AttrComp->BaseLongswordXPMod);
    else if (Prefix == TEXT("gre")) AddBonus(AttrComp->BonusGreatswordXPMod, AttrComp->BaseGreatswordXPMod);
    else if (Prefix == TEXT("shi")) AddBonus(AttrComp->BonusDefense, AttrComp->BaseDefense);
    else if (Prefix == TEXT("log")) AddBonus(AttrComp->BonusLongswordDamageMod, AttrComp->BaseLongswordDamageMod);
    else if (Prefix == TEXT("grg")) AddBonus(AttrComp->BonusGreatswordDamageMod, AttrComp->BaseGreatswordDamageMod);
    else if (Prefix == TEXT("min")) AddBonus(AttrComp->BonusDamageToLowHPEnemies, AttrComp->BaseDamageToLowHPEnemies);
    else if (Prefix == TEXT("max")) AddBonus(AttrComp->BonusDamageToHighHPEnemies, AttrComp->BaseDamageToHighHPEnemies);
    else if (Prefix == TEXT("nor")) AddBonus(AttrComp->BonusDamageToNormalEnemies, AttrComp->BaseDamageToNormalEnemies);
    else if (Prefix == TEXT("bos")) AddBonus(AttrComp->BonusDamageToBoss, AttrComp->BaseDamageToBoss);
    else if (Prefix == TEXT("las")) AttrComp->bHasLastStandSkill = true;
}
