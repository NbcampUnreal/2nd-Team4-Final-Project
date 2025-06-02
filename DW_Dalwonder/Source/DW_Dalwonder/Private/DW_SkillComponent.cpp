#include "DW_SkillComponent.h"
#include "DW_AttributeComponent.h"
#include "GameFramework/Character.h"

UDW_SkillComponent::UDW_SkillComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

/* ------------------------------ 내부 헬퍼 ------------------------------ */
FSkillState* UDW_SkillComponent::FindSkillState(FName SkillID)
{
    return SkillStateMap.Find(SkillID);
}

/* ------------------------------ 공개 API ------------------------------ */
bool UDW_SkillComponent::TryLearnSkill(FName SkillID)
{
    if (!SkillDataTable) return false;

    const FSkillData* SkillData = SkillDataTable->FindRow<FSkillData>(SkillID, TEXT("TryLearnSkill"));
    if (!SkillData) return false;

    /*  ── 선행 조건 ── */
    if (!SkillData->PrerequisiteSkillID.IsNone())
    {
        const FSkillState* PreState = SkillStateMap.Find(SkillData->PrerequisiteSkillID);
        if (!PreState || PreState->CurrentLevel <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Prerequisite skill not learned"));
            return false;
        }
    }

    /* ── 현재 상태 ── */
    FSkillState* MyState = SkillStateMap.Find(SkillID);
    if (!MyState)           // 첫 습득
    {
        if (CurrentSP < SkillData->Cost) return false;

        FSkillState NewState{ SkillID, 1 };
        SkillStateMap.Add(SkillID, NewState);
        CurrentSP -= SkillData->Cost;

        ApplySkillEffect(*SkillData, 1);
    }
    else                    // 레벨업
    {
        if (MyState->CurrentLevel >= SkillData->MaxLevel) return false;
        if (CurrentSP < SkillData->Cost) return false;

        MyState->CurrentLevel++;
        CurrentSP -= SkillData->Cost;

        ApplySkillEffect(*SkillData, 1);
    }

    OnSkillUpdated.Broadcast();
    return true;
}

int32 UDW_SkillComponent::GetSkillLevel(FName SkillID) const
{
    const FSkillState* State = SkillStateMap.Find(SkillID);
    return State ? State->CurrentLevel : 0;
}

/* --------------------------- 효과 적용부 --------------------------- */
void UDW_SkillComponent::ApplySkillEffect(const FSkillData& SkillData, int32 DeltaLevel)
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;

    UDW_AttributeComponent* AttrComp = OwnerActor->FindComponentByClass<UDW_AttributeComponent>();
    if (!AttrComp) return;

    const FString Prefix = SkillData.SkillID.ToString().Left(3).ToLower();
    const float   RawInc = static_cast<float>(SkillData.Increase) * DeltaLevel;

    auto AddBonus = [&](float& BonusField, float BaseField)
        {
            if (SkillData.IncreaseType == 1)       // % 증가
                BonusField += BaseField * (RawInc / 100.f);
            else                                   // 절댓값
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
}
