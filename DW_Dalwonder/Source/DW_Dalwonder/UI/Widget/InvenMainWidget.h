// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvenMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UInvenMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

protected:
    // Left Panel
    UPROPERTY(meta = (BindWidget))
    class UButton* WeaponButton;

    UPROPERTY(meta = (BindWidget))
    class UImage* WeaponIcon;

    UPROPERTY(meta = (BindWidget))
    class UButton* ItemButton;

    UPROPERTY(meta = (BindWidget))
    class UImage* ItemIcon;

    UPROPERTY(meta = (BindWidget))
    class UButton* MaterialButton;

    UPROPERTY(meta = (BindWidget))
    class UImage* MaterialIcon;

    // Center Character View (Use Viewport or Render Target in Widget)

    // Right Panel
    UPROPERTY(meta = (BindWidget))
    class UButton* HelmetSlot;

    UPROPERTY(meta = (BindWidget))
    class UImage* HelmetIcon;

    UPROPERTY(meta = (BindWidget))
    class UButton* ChestSlot;

    UPROPERTY(meta = (BindWidget))
    class UImage* ChestIcon;

    UPROPERTY(meta = (BindWidget))
    class UButton* PantsSlot;

    UPROPERTY(meta = (BindWidget))
    class UImage* PantsIcon;

    UPROPERTY(meta = (BindWidget))
    class UButton* BootsSlot;

    UPROPERTY(meta = (BindWidget))
    class UImage* BootsIcon;

    // Bottom Left (HP)
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HPLabel;

    // Bottom Right (Armor)
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ArmorLabel;
};
