#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DW_SkillTree.generated.h"

class UCanvasPanel;
class UDW_SkillComponent;
class UDW_SkillIcon;

UCLASS()
class DW_DALWONDER_API UDW_SkillTree : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UCanvasPanel* SkillCanvasPanel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
    UDW_SkillComponent* SkillComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
    TSubclassOf<UDW_SkillIcon> SkillIconClass;

private:
    void CreateSkillIcons();
    UFUNCTION()
    void RefreshAllIcons();

public:
    UFUNCTION()
    void UpdateSkillActivationStates();
};