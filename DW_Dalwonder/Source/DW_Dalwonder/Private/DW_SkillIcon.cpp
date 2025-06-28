#include "DW_SkillIcon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "DW_SkillComponent.h"
#include "DW_SkillTree.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/Widget/DW_SkillTooltip.h"
#include "DW_SkillManager.h"


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
        //이펙트가 클릭 안가리게
        DotEffectImage->SetVisibility(ESlateVisibility::HitTestInvisible);
    }

    UpdateIcon();
}

void UDW_SkillIcon::OnSkillDoubleClicked()
{
    if (!SkillComponent || !bCanActivate) return; // 클릭 제한

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

    if (SkillID.IsNone()) return;

    const int32 Level = SkillComponent->GetSkillLevel(SkillID);
    bUnlocked = Level > 0;

    const FSkillData* SkillData = SkillManager ? SkillManager->GetSkillData(SkillID) : nullptr;

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
            DotEffectImage->SetVisibility(ESlateVisibility::HitTestInvisible);

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

    // 상태 판단 및 색상 갱신
    if (SkillManager)
    {
        const TMap<FName, FSkillState>& SkillStates = SkillComponent->SkillStateMap;

        bool bEnable = true;
        if (!bUnlocked) // 스킬을 아직 배우지 않은 경우
        {
            bEnable = SkillManager->CanUnlockSkill(SkillID, SkillStates);
        }

        SetCanActivate(bEnable);
    }
}

void UDW_SkillIcon::SetCanActivate(bool bEnable)
{
    if (IconImage)
    {
        IconImage->SetRenderOpacity(bEnable ? 1.0f : 0.4f);
    }

    // 항상 true로 유지 (비활성화하면 자식도 같이 알파값 손상)
    if (SkillButton)
    {
        SkillButton->SetIsEnabled(true);
    }

    bCanActivate = bEnable; // 상태 저장해서 클릭 막기용
}

void UDW_SkillIcon::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
    if (TooltipWidgetClass && SkillManager)
    {
        const FSkillData* Data = SkillManager->GetSkillData(SkillID);
        if (!Data) return;

        ActiveTooltip = CreateWidget<UDW_SkillTooltip>(GetWorld(), TooltipWidgetClass);
        if (ActiveTooltip)
        {
            int32 Level = SkillComponent->GetSkillLevel(SkillID);
            ActiveTooltip->SetSkillTooltipInfo(Data->SkillName, Data->Description, Level, Data->MaxLevel);

            FVector2D MouseScreenPos = InMouseEvent.GetScreenSpacePosition();
            FVector2D Offset(1.f, 1.f); // 마우스 오른쪽 아래

            ActiveTooltip->AddToViewport(999); // 높은 ZOrder
            ActiveTooltip->SetVisibility(ESlateVisibility::Visible);
            ActiveTooltip->SetPositionInViewport(MouseScreenPos + Offset, true); // DPI 무시
        }
    }
}

void UDW_SkillIcon::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);

    if (ActiveTooltip)
    {
        ActiveTooltip->RemoveFromParent();
        ActiveTooltip = nullptr;
    }
}
