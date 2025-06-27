#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingRowSlider.generated.h"

class UTextBlock;
class UButton;
class USlider;
class UEditableTextBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSliderValueChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSliderSaved, float, NewValue);

UCLASS()
class DW_DALWONDER_API USettingRowSlider : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	void InitSetting(const FText& InLabel, float InMin, float InMax, float InStep, int32 InDecimalPlaces,
	                 float InArrowStep,
	                 float InDefault);


	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnLeftClicked();

	UFUNCTION()
	void OnRightClicked();

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	void UpdateDisplayText(float Value);
	void SetSliderAndDisplay(float InValue);
	void SaveSettingValue();

	UPROPERTY(meta = (BindWidget))
	USlider* Slider_Value;

	UPROPERTY(BlueprintAssignable, Category="Event")
	FOnSliderSaved OnSliderSaved;

	UPROPERTY(BlueprintAssignable)
	FOnSliderValueChanged OnValueChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider")
	bool bHasSavedValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider")
	float SavedValue = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Slider")
	float CurrentValue = 0.f;
protected:

	// 설정 레이블 (디자인 타임용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider")
	FText Label;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_SettingLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Left;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Right;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TextBox_ValueDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider")
	float MinValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider")
	float MaxValue = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider")
	float StepSize = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider")
	int32 DecimalPlaces = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider")
	float ArrowStepSize = 5.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider", meta = (ClampMin = "0.0"))
	float DefaultValue = 1.0f;
};
