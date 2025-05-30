#include "DW_SkillTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "DW_SkillIcon.h"
#include "DW_SkillComponent.h"

void UDW_SkillTree::NativeConstruct()
{
    Super::NativeConstruct();

    if (SkillComponent)
    {
        SkillComponent->OnSkillUpdated.AddDynamic(this, &UDW_SkillTree::RefreshAllIcons);
    }

    CreateSkillIcons();
}

void UDW_SkillTree::CreateSkillIcons()
{
    if (!SkillCanvasPanel || !SkillIconClass || !SkillComponent) return;

    struct FSkillIconLayout
    {
        FName SkillID;
        FVector2D Position;
    };

    TArray<FSkillIconLayout> SkillLayoutList = {
        { "Fireball", FVector2D(100.f, 100.f) },
        { "FlameUp", FVector2D(300.f, 250.f) },
        { "Meteor", FVector2D(500.f, 400.f) },
    };

    for (const FSkillIconLayout& Entry : SkillLayoutList)
    {
        UDW_SkillIcon* SkillIcon = CreateWidget<UDW_SkillIcon>(this, SkillIconClass);
        if (!SkillIcon) continue;

        SkillIcon->SkillID = Entry.SkillID;
        SkillIcon->SkillComponent = SkillComponent;

        SkillCanvasPanel->AddChild(SkillIcon);

        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(SkillIcon->Slot))
        {
            CanvasSlot->SetAutoSize(true);
            CanvasSlot->SetPosition(Entry.Position);
        }
    }
}

void UDW_SkillTree::RefreshAllIcons()
{
    if (!SkillCanvasPanel) return;

    for (UWidget* Child : SkillCanvasPanel->GetAllChildren())
    {
        if (UDW_SkillIcon* Icon = Cast<UDW_SkillIcon>(Child))
        {
            Icon->UpdateIcon();
        }
    }
}