#include "DW_SkillTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "DW_SkillIcon.h"
#include "DW_SkillComponent.h"
#include "Character/DW_CharacterBase.h"

void UDW_SkillTree::NativeConstruct()
{
    Super::NativeConstruct();

    // 캐릭터에서 SkillComponent 자동 할당
    if (!SkillComponent)
    {
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            if (ADW_CharacterBase* DWChar = Cast<ADW_CharacterBase>(PC->GetPawn()))
            {
                SkillComponent = DWChar->FindComponentByClass<UDW_SkillComponent>();
            }
        }
    }

    if (SkillComponent)
    {
        SkillComponent->OnSkillUpdated.AddDynamic(this, &UDW_SkillTree::RefreshAllIcons);
    }

    CreateSkillIcons();
}

void UDW_SkillTree::CreateSkillIcons()
{
    if (!SkillCanvasPanel || !SkillIconClass)
        return;

    // 자동으로 캐릭터에서 SkillComponent 가져오기
    if (!SkillComponent || !SkillComponent->SkillDataTable)
    {
        return;
    }

    UDataTable* SkillDataTable = SkillComponent->SkillDataTable;
    static const FString ContextString(TEXT("CreateSkillIcons"));

    // -------------------------------
    // Step 1: SkillData 맵 구성
    // -------------------------------
    TMap<FName, const FSkillData*> SkillDataMap;
    for (const auto& RowPair : SkillDataTable->GetRowMap())
    {
        const FSkillData* SkillData = reinterpret_cast<const FSkillData*>(RowPair.Value);
        if (!SkillData) continue;
        SkillDataMap.FindOrAdd(SkillData->SkillID) = SkillData;
    }

    // -------------------------------
    // Step 2: 포인터 기반 트리 구조
    // -------------------------------
    struct FSkillTreeNode
    {
        FName SkillID;
        TArray<FSkillTreeNode*> Children;
    };

    TMap<FName, FSkillTreeNode*> NodeMap;

    for (const auto& Pair : SkillDataMap)
    {
        FSkillTreeNode* NewNode = new FSkillTreeNode();
        NewNode->SkillID = Pair.Key;
        NodeMap.Add(Pair.Key, NewNode);
    }

    for (const auto& Pair : SkillDataMap)
    {
        const FSkillData* Data = Pair.Value;
        FName ChildID = Data->SkillID;
        FName ParentID = Data->PrerequisiteSkillID;

        if (!ParentID.IsNone() && NodeMap.Contains(ParentID))
        {
            NodeMap[ParentID]->Children.Add(NodeMap[ChildID]);
        }
    }

    FSkillTreeNode* RootNode = NodeMap.Contains("Sta001") ? NodeMap["Sta001"] : nullptr;
    if (!RootNode) return;

    // -------------------------------
    // Step 3: 트리 너비 계산
    // -------------------------------
    int32 TreeWidth = 0;
    TFunction<void(FSkillTreeNode*)> CountNodes;
    CountNodes = [&](FSkillTreeNode* Node)
        {
            TreeWidth++;
            for (FSkillTreeNode* Child : Node->Children)
            {
                CountNodes(Child);
            }
        };
    CountNodes(RootNode);

    // -------------------------------
    // Step 4: 위치 배치 지그재그 위치 계산
    // -------------------------------
    TMap<FName, FVector2D> CalculatedPositions;

    int32 ZigzagIndex = 0;
    FVector2D CanvasCenter = FVector2D(960.f, 540.f);

    constexpr float HorizontalSpacing = 140.f;      // 오른쪽으로 이동 간격
    constexpr float VerticalAmplitude = 200.f;      // 위아래 흔들림
    constexpr float ZigZagFrequency = PI / 3.f;     // 파도 주기

    TFunction<void(FSkillTreeNode*)> TraverseZigZagHorizontal;
    TraverseZigZagHorizontal = [&](FSkillTreeNode* Node)
        {
            float X = ZigzagIndex * HorizontalSpacing;
            float Y = CanvasCenter.Y + FMath::Sin(ZigzagIndex * ZigZagFrequency) * VerticalAmplitude;

            FVector2D Pos = FVector2D(X + 100, Y); // 살짝 오른쪽 쉬프트
            CalculatedPositions.Add(Node->SkillID, Pos);

            ZigzagIndex++;

            for (FSkillTreeNode* Child : Node->Children)
            {
                TraverseZigZagHorizontal(Child);
            }
        };

    TraverseZigZagHorizontal(RootNode);


    // -------------------------------
    // Step 5: 아이콘 생성
    // -------------------------------
    TSet<FName> SpawnedIcons;

    for (const auto& Pair : CalculatedPositions)
    {
        FName RealID = Pair.Key;

        
        FName DisplayID = RealID;
        // UI 대검 중검 구분... 흠...
        /*if (RealID == "Lon001" || RealID == "Gre001")
        {
            DisplayID = "MeleeSkill";
            if (SpawnedIcons.Contains(DisplayID))
                continue;
        }*/

        UDW_SkillIcon* SkillIcon = CreateWidget<UDW_SkillIcon>(this, SkillIconClass);
        if (!SkillIcon) continue;

        SkillIcon->SkillID = DisplayID;
        SkillIcon->SkillComponent = SkillComponent;

        SkillCanvasPanel->AddChild(SkillIcon);

        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(SkillIcon->Slot))
        {
            CanvasSlot->SetAutoSize(true);
            CanvasSlot->SetPosition(Pair.Value);
        }

        // 아이콘 텍스처 설정
        const FSkillData* SkillData = SkillDataMap.Contains(RealID) ? SkillDataMap[RealID] : nullptr;
        if (SkillData && SkillData->Icon && SkillIcon->GetIconImage())
        {
            SkillIcon->GetIconImage()->SetBrushFromTexture(SkillData->Icon);
        }

        // 선행 조건에 따른 버튼 활성화 여부 설정
        if (SkillData)
        {
            bool bShouldEnable = true;

            if (!SkillData->PrerequisiteSkillID.IsNone())
            {
                int32 PrereqLevel = SkillComponent->GetSkillLevel(SkillData->PrerequisiteSkillID);
                bShouldEnable = PrereqLevel > 0;
            }

            SkillIcon->SetIsEnabled(bShouldEnable);
        }

        SkillIcon->UpdateIcon();
        SpawnedIcons.Add(DisplayID);
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

void UDW_SkillTree::UpdateSkillActivationStates()
{
    if (!SkillCanvasPanel || !SkillComponent || !SkillComponent->SkillDataTable)
        return;
    // 모든 스킬트리의 스킬 확인
    for (UWidget* Child : SkillCanvasPanel->GetAllChildren())
    {
        if (UDW_SkillIcon* SkillIcon = Cast<UDW_SkillIcon>(Child))
        {
            const FName& SkillID = SkillIcon->SkillID;
            const FSkillData* SkillData = SkillComponent->SkillDataTable->FindRow<FSkillData>(SkillID, TEXT("UpdateActivation"));

            if (SkillData && !SkillData->PrerequisiteSkillID.IsNone())
            {
                int32 PrereqLevel = SkillComponent->GetSkillLevel(SkillData->PrerequisiteSkillID);
                SkillIcon->SetIsEnabled(PrereqLevel > 0);
            }
        }
    }
}
