// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SavedGameWidget.generated.h"

class UCustomButtonWidget;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API USavedGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:

	// 버튼 클릭 이벤트 함수
	UFUNCTION()
	void OnBackClicked();

private:

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* SaveData1;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* SaveData2;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* SaveData3;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* SaveData4;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* BackButton;

};
