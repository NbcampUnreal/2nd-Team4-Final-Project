#include "DW_SkillComponent.h"

UDW_SkillComponent::UDW_SkillComponent()
{

}

bool UDW_SkillComponent::TryLearnSkill(FName SkillID)
{
    if (!SkillDataTable) return false;

    const FSkillData* SkillData = SkillDataTable->FindRow<FSkillData>(SkillID, "");
    if (!SkillData) return false;

    // ���� ���� üũ
    if (!SkillData->PrerequisiteSkillID.IsNone())
    {
        const FSkillState* PreState = SkillStateMap.Find(SkillData->PrerequisiteSkillID);
        if (!PreState || PreState->CurrentLevel <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("���� ��ų ����"));
            return false;
        }
    }

    // ���� ���� ��������
    FSkillState* MyState = SkillStateMap.Find(SkillID);
    if (!MyState)
    {
        if (CurrentSP < SkillData->Cost) return false;

        SkillStateMap.Add(SkillID, FSkillState{ SkillID, 1 });
        CurrentSP -= SkillData->Cost;
    }
    else
    {
        if (MyState->CurrentLevel >= SkillData->MaxLevel) return false;
        if (CurrentSP < SkillData->Cost) return false;

        MyState->CurrentLevel++;
        CurrentSP -= SkillData->Cost;
    }

    // �̺�Ʈ ��ε�ĳ��Ʈ (UI ���� ��)
    OnSkillUpdated.Broadcast();

    return true;
}

int32 UDW_SkillComponent::GetSkillLevel(FName SkillID) const
{
    const FSkillState* State = SkillStateMap.Find(SkillID);
    return State ? State->CurrentLevel : 0;
}