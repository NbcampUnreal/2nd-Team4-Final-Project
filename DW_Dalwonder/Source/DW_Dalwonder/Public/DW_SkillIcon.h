// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DW_SkillIcon.generated.h"

class UDW_SkillComponent;
class UImage;
class UTextBlock;
class UButton;

UCLASS()
class DW_DALWONDER_API UDW_SkillIcon : public UUserWidget
{
	GENERATED_BODY()

public:
    // 스킬 ID (DataTable Key)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    FName SkillID;

    // 플레이어의 스킬 컴포넌트
    UPROPERTY(BlueprintReadWrite)
    UDW_SkillComponent* SkillComponent;

protected:
    virtual void NativeConstruct() override;

    // UI 바인딩
    UPROPERTY(meta = (BindWidget))
    UImage* IconImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelText;

    UPROPERTY(meta = (BindWidget))
    UButton* SkillButton;

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

    // 잠금 상태 여부
    bool bUnlocked = false;

    // 내부 처리 함수
    UFUNCTION()
    void OnSkillDoubleClicked();

public:
    // 상태 갱신
    UFUNCTION(BlueprintCallable)
    void UpdateIcon();
};