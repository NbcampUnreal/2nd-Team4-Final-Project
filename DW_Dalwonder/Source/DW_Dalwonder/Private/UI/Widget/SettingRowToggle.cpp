#include "UI/Widget/SettingRowToggle.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"

void USettingRowToggle::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (TextLabel)
	{
		TextLabel->SetText(LabelText);
	}

	if (CheckBox)
	{
		CheckBox->SetIsChecked(bIsOn);
	}
}

void USettingRowToggle::NativeConstruct()
{
	Super::NativeConstruct();

	if (CheckBox)
	{
		CheckBox->OnCheckStateChanged.AddDynamic(this, &USettingRowToggle::HandleToggleChanged);
	}
}

void USettingRowToggle::HandleToggleChanged(bool bNewChecked)
{
	bIsOn = bNewChecked;
	OnToggleChanged.Broadcast(bIsOn);
}
