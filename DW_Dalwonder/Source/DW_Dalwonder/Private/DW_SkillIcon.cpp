#include "DW_SkillIcon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DW_SkillComponent.h"

void UDW_SkillIcon::NativeConstruct()
{
    Super::NativeConstruct();

    if (SkillButton)
    {
        // 더블클릭 처리 - 예시: OnClicked 대신 Blueprint에서 두 번 클릭 처리 가능
        SkillButton->OnClicked.AddDynamic(this, &UDW_SkillIcon::OnSkillDoubleClicked);
    }

    UpdateIcon();
}

void UDW_SkillIcon::OnSkillDoubleClicked()
{
    if (!SkillComponent) return;

    if (!bUnlocked)
    {
        bool bSuccess = SkillComponent->TryLearnSkill(SkillID);
        if (bSuccess)
        {
            UpdateIcon();
        }
    }
}

void UDW_SkillIcon::UpdateIcon()
{
    if (!SkillComponent) return;

    int32 Level = SkillComponent->GetSkillLevel(SkillID);
    bUnlocked = (Level > 0);

    if (IconImage)
    {
        IconImage->SetColorAndOpacity(
            bUnlocked ? FLinearColor::White : FLinearColor(0.3f, 0.3f, 0.3f)
        );
    }

    if (LevelText)
    {
        if (Level > 0)
            LevelText->SetText(FText::AsNumber(Level));
        else
            LevelText->SetText(FText::GetEmpty());
    }

    // 레벨 스팟 이미지 표시
    TArray<UImage*> LevelSpots = { LevelSpot01, LevelSpot02, LevelSpot03, LevelSpot04, LevelSpot05 };

    for (int32 i = 0; i < LevelSpots.Num(); ++i)
    {
        if (LevelSpots[i])
        {
            LevelSpots[i]->SetVisibility(i < Level ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }
}