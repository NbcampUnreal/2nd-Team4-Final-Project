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
        LevelText->SetText(Level > 0 ? FText::AsNumber(Level) : FText::GetEmpty());
    }

    TArray<UImage*> LevelSpots = { LevelSpot01, LevelSpot02, LevelSpot03, LevelSpot04, LevelSpot05 };
    for (int32 i = 0; i < LevelSpots.Num(); ++i)
    {
        if (LevelSpots[i])
        {
            LevelSpots[i]->SetVisibility(i < Level ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }
}
