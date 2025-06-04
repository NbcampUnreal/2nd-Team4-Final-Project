// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BackButtonWidget.generated.h"

class UCustomButtonWidget;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UBackButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void HandleBackClicked();

	UPROPERTY(meta = (BindWidget))
	UCustomButtonWidget* BackButton;
};
