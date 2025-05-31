#include "DW_SkillIcon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DW_SkillComponent.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

void UDW_SkillIcon::NativeConstruct()
{
    Super::NativeConstruct();

    if (SkillButton)
    {
        SkillButton->OnClicked.AddDynamic(this, &UDW_SkillIcon::OnSkillDoubleClicked);
    }

    if (DotEffectImage)
    {
        static ConstructorHelpers::FObjectFinder<UMaterialInterface> DotMat(TEXT("/Game/UI/Materials/M_Dot"));
        if (DotMat.Succeeded())
        {
            UMaterialInstanceDynamic* DotMID = UMaterialInstanceDynamic::Create(DotMat.Object, this);
            DotEffectImage->SetBrushFromMaterial(DotMID);
            DotEffectImage->SetVisibility(ESlateVisibility::Hidden);
        }
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

    UTexture2D* TextureToUse = bUnlocked ? SkillData->IconActivated : SkillData->Icon;
    if (TextureToUse)
    {
        IconImage->SetBrushFromTexture(TextureToUse);
    }

    if (LevelText)
    {
        LevelText->SetText(FText::AsNumber(Level));
    }

    TArray<UImage*> LevelSpots = { LevelSpot01, LevelSpot02, LevelSpot03, LevelSpot04, LevelSpot05 };
    for (int32 i = 0; i < LevelSpots.Num(); ++i)
    {
        if (LevelSpots[i])
        {
            LevelSpots[i]->SetVisibility(i < Level ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }

    if (DotEffectImage)
    {
        DotEffectImage->SetVisibility(bUnlocked ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}
