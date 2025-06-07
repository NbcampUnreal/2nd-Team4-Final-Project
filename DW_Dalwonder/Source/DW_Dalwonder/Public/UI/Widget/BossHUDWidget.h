// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UBossHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitBossHUD(const FString& BossName, float MaxHP);

	UFUNCTION(BlueprintCallable)
	void UpdateHP(float NewHP);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BossNameText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BossHPBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BossHPText;

	float MaxBossHP = 0.f;
};
