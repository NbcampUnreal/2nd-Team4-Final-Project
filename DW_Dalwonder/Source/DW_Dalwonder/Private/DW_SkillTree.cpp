#include "DW_SkillTree.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "DW_SkillIcon.h"
#include "DW_SkillComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

void UDW_SkillTree::NativeConstruct()
{
    Super::NativeConstruct();

    CreateSkillIcons();
}

void UDW_SkillTree::CreateSkillIcons()
{
    if (!SkillCanvasPanel || !SkillIconClass || !SkillComponent) return;

    // ���÷� ���� ��ġ ���� (���ϴ� ��ġ�� ���� ���� ����)
    struct FSkillIconLayout
    {
        FName SkillID;
        FVector2D Position;
    };

    TArray<FSkillIconLayout> SkillLayoutList = {
        { "Fireball", FVector2D(100.f, 100.f) },
        { "FlameUp", FVector2D(300.f, 250.f) },
        { "Meteor", FVector2D(500.f, 400.f) },
        // ... �ʿ� �� �� �߰�
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
