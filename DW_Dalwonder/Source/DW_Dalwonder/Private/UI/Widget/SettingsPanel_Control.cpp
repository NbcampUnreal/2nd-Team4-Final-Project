#include "UI/Widget/SettingsPanel_Control.h"
#include "DW_GameInstance.h"
#include "Character/DW_PlayerController.h"
#include "UI/Widget/SettingsManager.h"
#include "UI/Widget/SettingRowSlider.h"
#include "Components/Slider.h"

void USettingsPanel_Control::NativeConstruct()
{
	Super::NativeConstruct();

	// ----- 마우스 감도 설정 ----- //
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

	// ----- 키 설정 ----- //
	
	// if (KeyBind_Attack)
	// {
	// 	KeyBind_Attack->ActionName = ADW_PlayerController::Action_Attack;
	// 	KeyBind_Attack->AssignedKey = EKeys::LeftMouseButton;
	// 	KeyBind_Attack->DisplayLabel = FText::FromString(TEXT("기본공격"));
	// 	KeyBind_Attack->OnKeyAssigned.AddDynamic(this, &USettingsPanel_Control::HandleKeyChanged);
	// }
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

void USettingsPanel_Control::HandleKeyChanged(FName ActionName, FKey NewKey)
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->SetCustomKey(ActionName, NewKey);
		}
	}
}
