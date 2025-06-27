#include "DW_SkillComponent.h"
#include "DW_AttributeComponent.h"
#include "DW_SkillManager.h"
#include "GameFramework/Character.h"

UDW_SkillComponent::UDW_SkillComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    if (AActor* Owner = GetOwner())
    {
        AttrCom = Owner->FindComponentByClass<UDW_AttributeComponent>();
        if (!AttrCom)
        {
            UE_LOG(LogTemp, Error, TEXT("AttributeComponent not found on %s"), *Owner->GetName());
        }
    }
}

/* ------------------------------ 저장데이터 불러오기 ------------------------------ */
void UDW_SkillComponent::ApplyAllSkillBonuses(UDW_AttributeComponent* AttributeComponent)
{
    if (!AttributeComponent || !SkillDataTable) return;

    for (const auto& Elem : SkillStateMap)
    {
        const FName& SkillID = Elem.Key;
        const FSkillState& SkillState = Elem.Value;

        // 레벨이 0 이하인 경우 보너스 없음
        if (SkillState.CurrentLevel <= 0) continue;

        // SkillDataTable에서 해당 스킬 데이터 조회
        const FSkillData* SkillData = SkillDataTable->FindRow<FSkillData>(SkillID, TEXT("Skill Bonus Apply"));
        if (!SkillData) continue;

        // Skill 레벨만큼 보너스 효과 적용
        ApplySkillEffect(*SkillData, SkillState.CurrentLevel);
    }
}

/* ------------------------------ 스킬 상태 ------------------------------ */
FSkillState* UDW_SkillComponent::FindSkillState(FName SkillID)
{
    return SkillStateMap.Find(SkillID);
}

/* ------------------------------ 스킬 API ------------------------------ */
bool UDW_SkillComponent::TryLearnSkill(FName SkillID)
{
    AActor* Owner = GetOwner();
    if (!Owner || !SkillDataTable) return false;

    UDW_AttributeComponent* Attr = Owner->FindComponentByClass<UDW_AttributeComponent>();
    if (!Attr) return false;

    // Lazy Init
    static UDW_SkillManager* SkillMgr = NewObject<UDW_SkillManager>(this);
    SkillMgr->Initialize(SkillDataTable);

    if (!SkillMgr->TryLearnSkill(SkillID, CurrentSP, SkillStateMap, Attr))
        return false;

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
    if (!AttrCom || !SkillDataTable) return;

    static UDW_SkillManager* SkillMgr = NewObject<UDW_SkillManager>(this);
    SkillMgr->Initialize(SkillDataTable);

    for (const auto& Elem : SkillStateMap)
    {
        const int32 Level = Elem.Value.CurrentLevel;
        if (Level <= 0) continue;

        const FSkillData* SkillDataPtr = SkillMgr->GetSkillData(Elem.Key);
        if (SkillDataPtr)
        {
            SkillMgr->ApplySkillEffect(*SkillDataPtr, Level, AttrCom);
        }
    }
}
