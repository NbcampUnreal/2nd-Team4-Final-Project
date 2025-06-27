#include "DW_SkillComponent.h"
#include "DW_AttributeComponent.h"
#include "DW_SkillManager.h"
#include "GameFramework/Character.h"
#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"

UDW_SkillComponent::UDW_SkillComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    if (AActor* Owner = GetOwner())
    {
        AttrCom = Owner->FindComponentByClass<UDW_AttributeComponent>();
    }
}

/* ------------------------------ 저장데이터 불러오기 ------------------------------ */
void UDW_SkillComponent::ApplyAllSkillBonuses(UDW_AttributeComponent* AttributeComponent)
{
    if (!AttributeComponent) return;

    // SkillManager를 GameMode에서 가져오기
    ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this));
    if (!GM || !GM->GetSkillManager()) return;

    UDW_SkillManager* SkillManager = GM->GetSkillManager();
    UDataTable* SkillDataTable = SkillManager->GetSkillDataTable();
    if (!SkillDataTable) return;

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
    if (!Owner) return false;

    UDW_AttributeComponent* Attr = Owner->FindComponentByClass<UDW_AttributeComponent>();
    if (!Attr) return false;

    if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
    {
        if (UDW_SkillManager* SkillMgr = GM->GetSkillManager())
        {
            const bool bSuccess = SkillMgr->TryLearnSkill(SkillID, CurrentSP, SkillStateMap, Attr);
            if (bSuccess)
            {
                OnSkillUpdated.Broadcast();
            }
            return bSuccess;
        }
    }

    return false;
}

int32 UDW_SkillComponent::GetSkillLevel(FName SkillID) const
{
    const FSkillState* State = SkillStateMap.Find(SkillID);
    return State ? State->CurrentLevel : 0;
}

void UDW_SkillComponent::IncreaseMastery(int32 NewMastery)
{
    CurrentMastery += NewMastery;

    while (CurrentMastery >= MaxMastery)
    {
        CurrentMastery -= MaxMastery;
        LevelUpCount++;
        MaxMastery *= 1.1f;
        TryLevelUp();
    }
}

void UDW_SkillComponent::TryLevelUp()
{
    if (LevelUpCount <= 50)
    {
        CurrentSP++;
    }
    else
    {
        CurrentSP += 2;
    }
}

/* --------------------------- 효과 적용부 --------------------------- */
void UDW_SkillComponent::ApplySkillEffect(const FSkillData& SkillData, int32 DeltaLevel)
{
    if (!AttrCom) return;

    if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
    {
        if (UDW_SkillManager* SkillMgr = GM->GetSkillManager())
        {
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
    }
}
