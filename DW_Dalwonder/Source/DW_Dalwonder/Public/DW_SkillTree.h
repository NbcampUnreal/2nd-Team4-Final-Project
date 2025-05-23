#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DW_SkillTree.generated.h"

class UScrollBox;
class UCanvasPanel;
class UDW_SkillIcon;

UCLASS()
class DW_DALWONDER_API UDW_SkillTree : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    // ��ũ�� ����
    UPROPERTY(meta = (BindWidget))
    UScrollBox* SkillScrollBox;

    // ���� SkillIcon�� ��ġ�� ĵ���� �г�
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* SkillCanvasPanel;

    // ������ ������ Ŭ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
    TSubclassOf<UDW_SkillIcon> SkillIconClass;

    // SkillComponent ����
    UPROPERTY(BlueprintReadWrite, Category = "SkillTree")
    class UDW_SkillComponent* SkillComponent;

    // ��ų ������ ������ �Լ�
    void CreateSkillIcons();
};
