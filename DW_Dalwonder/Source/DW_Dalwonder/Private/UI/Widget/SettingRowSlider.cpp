#include "UI/Widget/SettingRowSlider.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void USettingRowSlider::NativeConstruct()
{
	Super::NativeConstruct();

	float InitVal = bHasSavedValue ? SavedValue : DefaultValue;
	CurrentValue = InitVal;

	if (Slider_Value)
	{
		Slider_Value->SetMinValue(MinValue);
		Slider_Value->SetMaxValue(MaxValue);
		Slider_Value->SetStepSize(StepSize);
		Slider_Value->SetValue(CurrentValue);
		Slider_Value->OnValueChanged.AddDynamic(this, &USettingRowSlider::HandleSliderValueChanged);
	}

	UpdateDisplayText(CurrentValue);

	if (TextBox_ValueDisplay)
	{
		TextBox_ValueDisplay->SetText(FText::AsNumber(CurrentValue));
		TextBox_ValueDisplay->OnTextCommitted.AddDynamic(this, &USettingRowSlider::OnTextCommitted);
	}

	if (Button_Left)
	{
		Button_Left->OnClicked.AddDynamic(this, &USettingRowSlider::OnLeftClicked);
	}
	if (Button_Right)
	{
		Button_Right->OnClicked.AddDynamic(this, &USettingRowSlider::OnRightClicked);
	}
}

void USettingRowSlider::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (Text_SettingLabel)
	{
		Text_SettingLabel->SetText(Label);
	}

	if (IsDesignTime())
	{
		CurrentValue = DefaultValue;

		if (Slider_Value)
		{
			Slider_Value->SetMinValue(MinValue);
			Slider_Value->SetMaxValue(MaxValue);
			Slider_Value->SetStepSize(StepSize);
			Slider_Value->SetValue(DefaultValue);
		}

		UpdateDisplayText(DefaultValue);
	}
}

void USettingRowSlider::InitSetting(const FText& InLabel, float InMin, float InMax, float InStep, int32 InDecimalPlaces, float InArrowStep, float InDefault)
{
	Label = InLabel;
	MinValue = InMin;
	MaxValue = InMax;
	StepSize = InStep;
	DecimalPlaces = InDecimalPlaces;
	ArrowStepSize = InArrowStep;

	bHasSavedValue = true;
	SavedValue = InDefault;
	CurrentValue = FMath::Clamp(SavedValue, MinValue, MaxValue);

	if (Text_SettingLabel)
	{
		Text_SettingLabel->SetText(Label);
	}

	if (Slider_Value)
	{
		Slider_Value->SetMinValue(MinValue);
		Slider_Value->SetMaxValue(MaxValue);
		Slider_Value->SetStepSize(StepSize);
		Slider_Value->SetValue(CurrentValue);
	}

	UpdateDisplayText(CurrentValue);
}

void USettingRowSlider::HandleSliderValueChanged(float NewValue)
{
	CurrentValue = NewValue;
	UpdateDisplayText(NewValue);
	OnSliderValueChanged.Broadcast(NewValue);
	OnSliderSaved.Broadcast(NewValue);
}

void USettingRowSlider::OnLeftClicked()
{
	CurrentValue = FMath::Clamp(CurrentValue - ArrowStepSize, MinValue, MaxValue);
	SetSliderAndDisplay(CurrentValue);
	OnSliderValueChanged.Broadcast(CurrentValue);
	OnSliderSaved.Broadcast(CurrentValue);
}

void USettingRowSlider::OnRightClicked()
{
	CurrentValue = FMath::Clamp(CurrentValue + ArrowStepSize, MinValue, MaxValue);
	SetSliderAndDisplay(CurrentValue);
	OnSliderValueChanged.Broadcast(CurrentValue);
	OnSliderSaved.Broadcast(CurrentValue);
}

void USettingRowSlider::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	const FString Str = Text.ToString();
	if (Str.IsNumeric())
	{
		float ParsedValue = FCString::Atof(*Str);
		CurrentValue = FMath::Clamp(ParsedValue, MinValue, MaxValue);
		SetSliderAndDisplay(CurrentValue);
		OnSliderValueChanged.Broadcast(CurrentValue);
		OnSliderSaved.Broadcast(CurrentValue);
	}
}

void USettingRowSlider::UpdateDisplayText(float Value)
{
	FString ValueString;
	if (DecimalPlaces <= 0)
	{
		ValueString = FString::FromInt(FMath::RoundToInt(Value));
	}
	else
	{
		ValueString = FString::Printf(TEXT("%.*f"), DecimalPlaces, Value);
	}

	if (TextBox_ValueDisplay)
	{
		TextBox_ValueDisplay->SetText(FText::FromString(ValueString));
	}
}

void USettingRowSlider::SetSliderAndDisplay(float InValue)
{
	CurrentValue = FMath::Clamp(InValue, MinValue, MaxValue);
	if (Slider_Value)
	{
		Slider_Value->SetValue(CurrentValue);
	}
	UpdateDisplayText(CurrentValue);
}
