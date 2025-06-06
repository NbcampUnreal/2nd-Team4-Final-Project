#include "DW_SkillIcon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DW_SkillComponent.h"
#include "DW_SkillTree.h"
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

    if (DotEffectImage && DotMaterial)
    {
        UMaterialInstanceDynamic* DotMID = UMaterialInstanceDynamic::Create(DotMaterial, this);
        DotEffectImage->SetBrushFromMaterial(DotMID);
        //DotEffectImage->SetVisibility(ESlateVisibility::Hidden);
        //이펙트가 클릭 안가리게
        DotEffectImage->SetVisibility(ESlateVisibility::HitTestInvisible);
    }

    UpdateIcon();
}

void UDW_SkillIcon::OnSkillDoubleClicked()
{
    if (!SkillComponent) return;

    const bool bSuccess = SkillComponent->TryLearnSkill(SkillID);
    if (bSuccess)
    {
        UpdateIcon();

        // 스킬들 선행 조건 확인 후 버튼 활성화 시켜주기
        if (UDW_SkillTree* SkillTree = GetTypedOuter<UDW_SkillTree>())
        {
            SkillTree->UpdateSkillActivationStates();
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
        if (bUnlocked)
        {
            DotEffectImage->SetVisibility(ESlateVisibility::Visible);

            // 1초 후 이펙트를 자동으로 숨김
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,
                FTimerDelegate::CreateWeakLambda(this, [this]()
                    {
                        if (DotEffectImage)
                        {
                            DotEffectImage->SetVisibility(ESlateVisibility::Hidden);
                        }
                    }),
                1.0f, // 1초 후 실행
                false // 반복 안함
            );
        }
        else
        {
            DotEffectImage->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // 최대 레벨 도달 시 버튼 비활성화
    if (SkillButton)
    {
        if (SkillData && Level >= SkillData->MaxLevel)
        {
            SkillButton->SetIsEnabled(false);
        }
        else
        {
            SkillButton->SetIsEnabled(true);
        }
    }
}
