// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ESCMenuWidget.generated.h"

class UCustomButtonWidget;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UESCMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnInfoClicked();

    UFUNCTION()
    void OnSkillTreeClicked();

    UFUNCTION()
    void OnInventoryClicked();

    UFUNCTION()
    void OnSaveClicked();

    UFUNCTION()
    void OnOptionClicked();

    UFUNCTION()
    void OnExitClicked();
	
protected:
    

    UPROPERTY(meta = (BindWidget))
    class UCustomButtonWidget* InfoButton;

    UPROPERTY(meta = (BindWidget))
    class UCustomButtonWidget* SkillTreeButton;

    UPROPERTY(meta = (BindWidget))
    class UCustomButtonWidget* InventoryButton;

    UPROPERTY(meta = (BindWidget))
    class UCustomButtonWidget* SaveButton;

    UPROPERTY(meta = (BindWidget))
    class UCustomButtonWidget* OptionButton;

    UPROPERTY(meta = (BindWidget))
    class UCustomButtonWidget* ExitButton;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> PlayerInfoWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> SkillTreeWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> InventoryWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> OptionMenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> SavedGameWidgetClass;
};
