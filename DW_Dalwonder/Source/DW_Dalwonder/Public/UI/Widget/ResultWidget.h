// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// UFUNCTION(BlueprintCallable)
	// void SetLetterSpacing(int32 Spacing);

	UFUNCTION(BlueprintCallable)
	void SetResultText(const FString& InText);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultTextShadow;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultText;
};
