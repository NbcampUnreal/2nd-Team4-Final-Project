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
    // 스크롤 영역
    UPROPERTY(meta = (BindWidget))
    UScrollBox* SkillScrollBox;

    // 실제 SkillIcon을 배치할 캔버스 패널
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* SkillCanvasPanel;

    // 스폰할 아이콘 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
    TSubclassOf<UDW_SkillIcon> SkillIconClass;

    // SkillComponent 참조
    UPROPERTY(BlueprintReadWrite, Category = "SkillTree")
    class UDW_SkillComponent* SkillComponent;

    // 스킬 아이콘 생성용 함수
    void CreateSkillIcons();
};
