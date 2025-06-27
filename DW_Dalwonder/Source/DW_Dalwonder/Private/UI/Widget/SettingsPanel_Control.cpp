#include "UI/Widget/SettingsPanel_Control.h"
#include "DW_GameInstance.h"
#include "UI/Widget/SettingsManager.h"
#include "UI/Widget/SettingRowSlider.h"
#include "Components/Slider.h"

void USettingsPanel_Control::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("Control Panel NativeConstruct"));

	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			// 저장된 값으로 직접 세팅
			float Saved = SM->GetMouseSensitivity();
			MouseSensitivity->Slider_Value->SetValue(Saved); // 직접 세팅
			MouseSensitivity->CurrentValue = Saved;
			MouseSensitivity->UpdateDisplayText(Saved);
		}
	}

	MouseSensitivity->OnSliderSaved.AddDynamic(this, &USettingsPanel_Control::HandleMouseSensitivityChanged);
}


void USettingsPanel_Control::NativeDestruct()
{
	Super::NativeDestruct();
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->SaveToSlot();
		}
	}
}

void USettingsPanel_Control::HandleMouseSensitivityChanged(float NewValue)
{
	const UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>();
	if (USettingsManager* SM = GI->GetSettingsManager())
	{
		SM->SetMouseSensitivity(NewValue);
	}
}
