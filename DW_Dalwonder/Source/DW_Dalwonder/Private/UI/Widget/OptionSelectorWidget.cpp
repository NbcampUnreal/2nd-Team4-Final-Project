#include "UI/Widget/OptionSelectorWidget.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"

void UOptionSelectorWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (TextLabel)
		TextLabel->SetText(LabelText);
}

void UOptionSelectorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ComboBoxOptions)
	{
		ComboBoxOptions->ClearOptions();
		for (const FText& Option : OptionList)
		{
			ComboBoxOptions->AddOption(Option.ToString());
		}

		ComboBoxOptions->OnSelectionChanged.AddDynamic(this, &UOptionSelectorWidget::OnOptionChanged);
	}
}

void UOptionSelectorWidget::OnOptionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	for (int32 i = 0; i < OptionList.Num(); ++i)
	{
		if (OptionList[i].ToString() == SelectedItem)
		{
			CurrentSelectedIndex = i;
			OnOptionSelected.Broadcast(i);
			break;
		}
	}
}
