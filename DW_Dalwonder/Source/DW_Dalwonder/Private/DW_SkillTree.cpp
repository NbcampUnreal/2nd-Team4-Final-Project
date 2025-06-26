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
    if (!SkillCanvasPanel || !SkillIconClass) return;
    if (!SkillComponent || !SkillComponent->SkillDataTable) return;

    UDataTable* SkillDataTable = SkillComponent->SkillDataTable;
    TMap<FName, const FSkillData*> SkillDataMap;

    // -------------------------------
    // Step 1: SkillData 맵 구성
    // -------------------------------
    for (const auto& Row : SkillDataTable->GetRowMap())
    {
        const FSkillData* Data = reinterpret_cast<const FSkillData*>(Row.Value);
        if (!Data) continue;
        SkillDataMap.Add(Row.Key, Data);
    }

    // -------------------------------
    // Step 2: Node 트리 구조체
    // -------------------------------
    struct FSkillTreeNode
    {
        FName SkillID;
        TArray<FSkillTreeNode*> Children;
    };

    TMap<FName, FSkillTreeNode*> NodeMap;
    for (const auto& Pair : SkillDataMap)
    {
        FSkillTreeNode* Node = new FSkillTreeNode();
        Node->SkillID = Pair.Key;
        NodeMap.Add(Pair.Key, Node);
    }

    // 부모 → 자식 연결 (복수 부모 지원: "A or B" 형식)
    for (const auto& Pair : SkillDataMap)
    {
        const FSkillData* Data = Pair.Value;
        FName ChildID = Pair.Key;
        FString Raw = Data->PrerequisiteSkillID.ToString();
        TArray<FString> Parents;

        if (Raw.Contains(" or "))
            Raw.ParseIntoArray(Parents, TEXT(" or "), true);
        else if (!Raw.IsEmpty())
            Parents.Add(Raw);

        for (const FString& P : Parents)
        {
            FName ParentID(*P.TrimStartAndEnd());
            if (NodeMap.Contains(ParentID))
                NodeMap[ParentID]->Children.Add(NodeMap[ChildID]);
        }
    }

    // -------------------------------
    // Step 3: 루트 노드 지정
    // -------------------------------
    TArray<FSkillTreeNode*> RootNodes;
    for (const auto& Pair : SkillDataMap)
    {
        const FSkillData* Data = Pair.Value;
        FString Raw = Data->PrerequisiteSkillID.ToString().TrimStartAndEnd();

		// 선행조건 "None" 또는 "Null"인 경우 루트 노드로 간주
        if (Raw.IsEmpty() || Raw.Equals("None", ESearchCase::IgnoreCase) || Raw.Equals("Null", ESearchCase::IgnoreCase))
        {
            RootNodes.Add(NodeMap[Pair.Key]);
        }
    }
	// 루트 노드가 없으면 종료
    if (RootNodes.Num() == 0) return;
	
    // -------------------------------
    // Step 4: 트리형 배치 계산
    // -------------------------------
    TMap<FName, FVector2D> CalculatedPositions;
    TMap<FName, FVector2D> TrueParentPosition; // 기준 부모 위치

	// 재귀 함수로 트리 구조를 순회하며 위치 계산
    TFunction<void(FSkillTreeNode*, FVector2D)> AssignPosition;
    AssignPosition = [&](FSkillTreeNode* Node, FVector2D Pos)
        {
            CalculatedPositions.Add(Node->SkillID, Pos);

            int32 Count = Node->Children.Num();
            float VerticalSpacing = 150.f;
            float XOffset = 240.f;
            float YStart = Pos.Y - ((Count - 1) * VerticalSpacing) / 2.f;

            for (int32 i = 0; i < Count; ++i)
            {
				// 선제 조건이 두개가 아니라면 기준 부모 위치를 사용
                FVector2D UsePos = TrueParentPosition.Contains(Node->SkillID)
                    ? TrueParentPosition[Node->SkillID]
                    : Pos;

                FVector2D ChildPos = FVector2D(UsePos.X + XOffset, YStart + i * VerticalSpacing);
                AssignPosition(Node->Children[i], ChildPos);
            }
        };

	// 루트 노드 위치 할당
    FVector2D Start = FVector2D(200, 540);
    for (int32 i = 0; i < RootNodes.Num(); ++i)
    {
        FVector2D RootPos = Start + FVector2D(0.f, i * 200.f);
        AssignPosition(RootNodes[i], RootPos);
    }

    // --------------------------------------------------
    // 5. 다중 부모 노드의 위치 보정 (부모 평균 + 오른쪽)
    // --------------------------------------------------
    for (const auto& Pair : SkillDataMap)
    {
        const FSkillData* Data = Pair.Value;
        FString Raw = Data->PrerequisiteSkillID.ToString();
        TArray<FString> Parents;
        Raw.ParseIntoArray(Parents, TEXT(" or "), true);

        if (Parents.Num() >= 2)
        {
            FVector2D Sum = FVector2D::ZeroVector;
            int32 Valid = 0;

            for (const FString& P : Parents)
            {
                FName PID(*P.TrimStartAndEnd());
                if (CalculatedPositions.Contains(PID))
                {
                    Sum += CalculatedPositions[PID];
                    Valid++;
                }
            }

            if (Valid > 0)
            {
                FVector2D Mid = Sum / Valid;
                FVector2D Final = Mid + FVector2D(240.f, 0.f); // 오른쪽 이동
                CalculatedPositions[Pair.Key] = Final;

                // 기준 좌표도 별도 저장 → 이후 흐름 유지
                TrueParentPosition.Add(Pair.Key, Final);
            }
        }
    }

    // -------------------------------
    // Step 5: 아이콘 생성
    // -------------------------------
    TSet<FName> Spawned;
    for (const auto& PosPair : CalculatedPositions)
    {
        FName ID = PosPair.Key;
        if (Spawned.Contains(ID)) continue;

        const FSkillData* Data = SkillDataMap[ID];
        UDW_SkillIcon* Icon = CreateWidget<UDW_SkillIcon>(this, SkillIconClass);
        if (!Icon || !Data) continue;

        Icon->SkillID = ID;
        Icon->SkillComponent = SkillComponent;
        SkillCanvasPanel->AddChild(Icon);

        if (UCanvasPanelSlot* CanSlot = Cast<UCanvasPanelSlot>(Icon->Slot))
        {
            CanSlot->SetAutoSize(true);
            CanSlot->SetPosition(PosPair.Value);
        }

        if (Data->Icon && Icon->GetIconImage())
            Icon->GetIconImage()->SetBrushFromTexture(Data->Icon);

        // 활성화 조건
        bool bEnable = true;
        FString Raw = Data->PrerequisiteSkillID.ToString();
        TArray<FString> Prereqs;

        if (Raw.Contains(" or "))
            Raw.ParseIntoArray(Prereqs, TEXT(" or "), true);
        else if (!Raw.IsEmpty() && !Raw.Equals("None", ESearchCase::IgnoreCase) && !Raw.Equals("Null", ESearchCase::IgnoreCase))
            Prereqs.Add(Raw);

        if (Prereqs.Num() > 0)
        {
            bEnable = false;
            for (const FString& P : Prereqs)
            {
                FName PID(*P.TrimStartAndEnd());
                if (SkillComponent->GetSkillLevel(PID) > 0)
                {
                    bEnable = true;
                    break;
                }
            }
        }

        Icon->SetIsEnabled(bEnable);
        Icon->UpdateIcon();
        Spawned.Add(ID);
    }

    // 선 연결
    for (const auto& Pair : SkillDataMap)
    {
        const FSkillData* Data = Pair.Value;
        FString Raw = Data->PrerequisiteSkillID.ToString();

        TArray<FString> Parents;
        if (Raw.Contains(" or "))
            Raw.ParseIntoArray(Parents, TEXT(" or "), true);
        else if (!Raw.IsEmpty())
            Parents.Add(Raw);

        for (const FString& P : Parents)
        {
            FName PID(*P.TrimStartAndEnd());
            if (CalculatedPositions.Contains(PID) && CalculatedPositions.Contains(Pair.Key))
            {
                FVector2D StartPos = CalculatedPositions[PID] + FVector2D(32.f, 32.f);
                FVector2D EndPos = CalculatedPositions[Pair.Key] + FVector2D(32.f, 32.f);
                CreateLineBetweenPoints(StartPos, EndPos);
            }
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

UImage* UDW_SkillTree::CreateLineBetweenPoints(const FVector2D& Start, const FVector2D& End)
{
    UImage* Line = NewObject<UImage>(this);
    if (!Line) return nullptr;

    FVector2D Dir = End - Start;
    float Length = Dir.Size();
    float AngleDeg = FMath::RadiansToDegrees(FMath::Atan2(Dir.Y, Dir.X));

    FSlateBrush Brush;
    Brush.TintColor = FLinearColor::White;
    Brush.ImageSize = FVector2D(Length, 2.f);

    Line->SetBrush(Brush);
    Line->SetRenderTransformPivot(FVector2D(0.f, 0.5f));
    Line->SetRenderTransform(FWidgetTransform(FVector2D::ZeroVector, FVector2D(1.f, 1.f), FVector2D::ZeroVector, AngleDeg));

    SkillCanvasPanel->AddChild(Line);
    if (UCanvasPanelSlot* CanSlot = Cast<UCanvasPanelSlot>(Line->Slot))
    {
        CanSlot->SetAutoSize(true);
        CanSlot->SetPosition(Start);
        CanSlot->SetZOrder(-1);
    }

    return Line;
}

FReply UDW_SkillTree::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bIsDragging = true;
        DragStartPosition = InMouseEvent.GetScreenSpacePosition();
        OriginalCanvasPosition = SkillCanvasPanel->RenderTransform.Translation;
        return FReply::Handled();
    }
    return FReply::Unhandled();
}

FReply UDW_SkillTree::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging)
    {
        FVector2D CurrentPos = InMouseEvent.GetScreenSpacePosition();
        FVector2D Delta = CurrentPos - DragStartPosition;
        SkillCanvasPanel->SetRenderTranslation(OriginalCanvasPosition + Delta);
        return FReply::Handled();
    }
    return FReply::Unhandled();
}

FReply UDW_SkillTree::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    bIsDragging = false;
    return FReply::Handled().ReleaseMouseCapture();
}
