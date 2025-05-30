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

    const int32 Level = SkillComponent->GetSkillLevel(SkillID);
    bUnlocked = Level > 0;

    const FSkillData* SkillData = SkillComponent->SkillDataTable
        ? SkillComponent->SkillDataTable->FindRow<FSkillData>(SkillID, TEXT("SkillIcon Update"))
        : nullptr;

    if (!SkillData || !IconImage) return;

    // 아이콘 텍스처 변경
    UTexture2D* TextureToUse = bUnlocked ? SkillData->IconActivated : SkillData->Icon;
    if (TextureToUse)
    {
        UE_LOG(LogTemp, Warning, TEXT("이미지 변경."));
        IconImage->SetBrushFromTexture(TextureToUse);
    }
    // 레벨 텍스트 및 점 UI 갱신
    if (LevelText)
    {
        LevelText->SetText(FText::AsNumber(Level));
    }

    /*int32 Level = SkillComponent->GetSkillLevel(SkillID);
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
    }*/

    TArray<UImage*> LevelSpots = { LevelSpot01, LevelSpot02, LevelSpot03, LevelSpot04, LevelSpot05 };
    for (int32 i = 0; i < LevelSpots.Num(); ++i)
    {
        if (LevelSpots[i])
        {
            UE_LOG(LogTemp, Warning, TEXT("젬 이미지 활성화"));
            LevelSpots[i]->SetVisibility(i < Level ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }
}
