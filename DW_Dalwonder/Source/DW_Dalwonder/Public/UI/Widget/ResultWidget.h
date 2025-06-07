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
	UFUNCTION(BlueprintCallable)
	void SetLetterSpacing(int32 Spacing);
	void StartLetterSpacingAnimation();
	void UpdateLetterSpacing();

	UFUNCTION(BlueprintCallable)
	void SetResultText(const FString& InText);

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ResultTextAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ResultTextShadowAnimation;

	FTimerHandle LetterSpacingTimerHandle;

	int32 InitialSpacing = 0;
	int32 TargetSpacing = 80;
	float LerpDuration = 0.5f;
	float ElapsedTime = 0.f;
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultTextShadow;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultText;
};
