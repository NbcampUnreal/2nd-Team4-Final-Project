// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    void UpdateHPBar(float Current, float Max);

    UFUNCTION(BlueprintCallable)
    void UpdateStaminaBar(float Current, float Max);

public:

    //체력바
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HPBar;

    //스테미너바
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* StaminaBar;

    //퀵슬롯 1~4
    UPROPERTY(meta = (BindWidget))
    class UImage* ItemSlot1;
    UPROPERTY(meta = (BindWidget))
    class UImage* ItemSlot2;
    UPROPERTY(meta = (BindWidget))
    class UImage* ItemSlot3;
    UPROPERTY(meta = (BindWidget))
    class UImage* ItemSlot4;
};
