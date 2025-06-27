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
    if (!Data) return false;

    if (Data->PrerequisiteSkillID.IsNone())
        return true;

    const FSkillState* PreState = SkillStateMap.Find(Data->PrerequisiteSkillID);
    return PreState && PreState->CurrentLevel > 0;
}

bool UDW_SkillManager::TryLearnSkill(FName SkillID, int32& InOutSP, TMap<FName, FSkillState>& SkillStateMap, UDW_AttributeComponent* AttrComp)
{
    const FSkillData* SkillData = GetSkillData(SkillID);
    if (!SkillData) return false;

    if (!CanUnlockSkill(SkillID, SkillStateMap)) return false;
    if (InOutSP < SkillData->Cost) return false;

    FSkillState& State = SkillStateMap.FindOrAdd(SkillID);
    if (State.CurrentLevel >= SkillData->MaxLevel) return false;

    State.CurrentLevel++;
    InOutSP -= SkillData->Cost;

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
