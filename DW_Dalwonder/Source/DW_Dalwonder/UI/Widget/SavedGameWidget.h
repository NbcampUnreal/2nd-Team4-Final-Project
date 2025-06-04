// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SavedGameWidget.generated.h"

UENUM(BlueprintType)
enum class ESaveGameWidgetMode : uint8
{
	Save,
	Load
};

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

	void SetMode(ESaveGameWidgetMode InMode);  // 모드 설정

private:

	// 버튼 클릭 이벤트 함수
	UFUNCTION()
	void OnBackClicked();

	//저장 슬롯 이벤트들
	UFUNCTION()
	void OnSaveSlot1Clicked();
	UFUNCTION()
	void OnSaveSlot2Clicked();
	UFUNCTION()
	void OnSaveSlot3Clicked();
	UFUNCTION()
	void OnSaveSlot4Clicked();

	void HandleSlotClicked(FString SlotName);

private:

	UPROPERTY()
	ESaveGameWidgetMode WidgetMode;

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

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TitleText;

};
