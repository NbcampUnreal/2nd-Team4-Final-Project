#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DW_SkillIcon.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UDW_SkillComponent;

UCLASS()
class DW_DALWONDER_API UDW_SkillIcon : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintReadWrite)
    FName SkillID;

    UPROPERTY(BlueprintReadWrite)
    UDW_SkillComponent* SkillComponent;

    UFUNCTION()
    void UpdateIcon();

    // Image Getter
    UImage* GetIconImage() const { return IconImage; }

protected:
    UFUNCTION()
    void OnSkillDoubleClicked();

    UPROPERTY(meta = (BindWidget))
    UButton* SkillButton;

    UPROPERTY(meta = (BindWidget))
    UImage* IconImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelText;

    UPROPERTY(meta = (BindWidget))
    UImage* LevelSpot01;

    UPROPERTY(meta = (BindWidget))
    UImage* LevelSpot02;

    UPROPERTY(meta = (BindWidget))
    UImage* LevelSpot03;

    UPROPERTY(meta = (BindWidget))
    UImage* LevelSpot04;

    UPROPERTY(meta = (BindWidget))
    UImage* LevelSpot05;

private:
    bool bUnlocked = false;
};
