// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DW_SkillTooltip.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDW_SkillTooltip : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void SetSkillTooltipInfo(const FText& Name, const FText& Desc, int32 CurrentLevel, int32 MaxLevel);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SkillNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SkillDescText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SkillLevelText;
};
