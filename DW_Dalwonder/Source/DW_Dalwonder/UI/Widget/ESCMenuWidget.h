// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ESCMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UESCMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UButton* InfoButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SkillTreeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* InventoryButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* OptionButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExitButton;
};
