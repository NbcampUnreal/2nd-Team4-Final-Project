#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DW_SkillTree.generated.h"

class UCanvasPanel;
class UDW_SkillComponent;
class UDW_SkillIcon;
class UDW_SkillManager;

UCLASS()
class DW_DALWONDER_API UDW_SkillTree : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    // Mouse Interaction
    FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
    FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
    FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
    FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UCanvasPanel* SkillCanvasPanel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
    UDW_SkillComponent* SkillComponent;

    UPROPERTY()
    UDW_SkillManager* SkillManager;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
    TSubclassOf<UDW_SkillIcon> SkillIconClass;

private:
    void CreateSkillIcons();

    UFUNCTION()
    void RefreshAllIcons();

    UFUNCTION()
    void ApplyZoom();

private:
    FVector2D DragStartPosition;
    FVector2D OriginalCanvasPosition;
    bool bIsDragging = false;

    float CurrentZoom = 1.0f;
    float MinZoom = 0.5f;
    float MaxZoom = 2.0f;

public:
    UFUNCTION()
    void UpdateSkillActivationStates();

    UFUNCTION()
    UImage* CreateLineBetweenPoints(const FVector2D& Start, const FVector2D& End);
};