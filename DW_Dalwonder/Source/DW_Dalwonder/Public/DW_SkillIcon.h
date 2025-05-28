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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    FName SkillID;

    UPROPERTY(BlueprintReadWrite)
    UDW_SkillComponent* SkillComponent;

protected:
    virtual void NativeConstruct() override;

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

    bool bUnlocked = false;

    UFUNCTION()
    void OnSkillDoubleClicked();

public:
    UFUNCTION(BlueprintCallable)
    void UpdateIcon();
};