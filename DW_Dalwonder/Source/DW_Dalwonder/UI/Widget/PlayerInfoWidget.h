// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

class UCustomButtonWidget;
class UImage;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	// 정보 업데이트
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerInfo(
		float MaxHP,
		float MaxStamina,
		float Damage,
		float Defense,
		float Weight,
		float MoveSpeed,
		float HPRegen,
		float StaminaRegen
	);
	
private:
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SteminaText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DefenceText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeightText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MoveSpeedText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPRegenText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StRegenText;

	//3D Model
	UPROPERTY(meta = (BindWidget))
	UImage* PreviewImage;
};
