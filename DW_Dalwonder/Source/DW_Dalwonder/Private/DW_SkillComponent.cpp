#include "DW_SkillComponent.h"

UDW_SkillComponent::UDW_SkillComponent()
{

}

bool UDW_SkillComponent::TryLearnSkill(FName SkillID)
{
    if (!SkillDataTable) return false;

    const FSkillData* SkillData = SkillDataTable->FindRow<FSkillData>(SkillID, "");
    if (!SkillData) return false;

    // 선행 조건 체크
    if (!SkillData->PrerequisiteSkillID.IsNone())
    {
        const FSkillState* PreState = SkillStateMap.Find(SkillData->PrerequisiteSkillID);
        if (!PreState || PreState->CurrentLevel <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("선행 스킬 부족"));
            return false;
        }
    }

    // 현재 상태 가져오기
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

    // 이벤트 브로드캐스트 (UI 갱신 등)
    OnSkillUpdated.Broadcast();

    return true;
}

int32 UDW_SkillComponent::GetSkillLevel(FName SkillID) const
{
    const FSkillState* State = SkillStateMap.Find(SkillID);
    return State ? State->CurrentLevel : 0;
}