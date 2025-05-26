// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenuWidgetBase.generated.h"

class UInventoryWidgetBase;
class UInventoryComponent;
UCLASS()
class DW_DALWONDER_API UInventoryMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MenuUI")
	UInventoryWidgetBase* GetInventoryWidget() const;


	UFUNCTION(BlueprintImplementableEvent, Category = "MenuUI")
	void UpdateInventoryUI(UInventoryComponent* InInventoryComponent);
	
protected:
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "UI Components")
	UInventoryWidgetBase* InventoryWidget; 
};
