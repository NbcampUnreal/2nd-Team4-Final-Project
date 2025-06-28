#include "UI/Widget/SettingsPanel_Sound.h"
#include "DW_GameInstance.h"
#include "UI/Widget/SettingsManager.h"
#include "UI/Widget/SettingRowSlider.h"

void USettingsPanel_Sound::NativeConstruct()
{
	Super::NativeConstruct();

	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			// 저장된 값 반영
			if (VolumeMaster)
			{
				VolumeMaster->SavedValue = SM->GetVolumeMaster();
				VolumeMaster->bHasSavedValue = true;
			}

			if (VolumeBGM)
			{
				VolumeBGM->SavedValue = SM->GetVolumeBGM();
				VolumeBGM->bHasSavedValue = true;
			}

			if (VolumeSFX)
			{
				VolumeSFX->SavedValue = SM->GetVolumeSFX();
				VolumeSFX->bHasSavedValue = true;
			}

			if (VolumeUI)
			{
				VolumeUI->SavedValue = SM->GetVolumeUI();
				VolumeUI->bHasSavedValue = true;
			}
		}
	}

	// 저장 이벤트 바인딩
	if (VolumeMaster)
	{
		VolumeMaster->OnSliderSaved.AddDynamic(this, &USettingsPanel_Sound::HandleVolumeMasterChanged);
		VolumeMaster->OnSliderValueChanged.AddDynamic(this, &USettingsPanel_Sound::ApplyVolumeMasterRuntime);
	}
	if (VolumeBGM)
	{
		VolumeBGM->OnSliderSaved.AddDynamic(this, &USettingsPanel_Sound::HandleVolumeBGMChanged);
		VolumeBGM->OnSliderValueChanged.AddDynamic(this, &USettingsPanel_Sound::ApplyVolumeBGMRuntime);
	}
	if (VolumeSFX)
	{
		VolumeSFX->OnSliderSaved.AddDynamic(this, &USettingsPanel_Sound::HandleVolumeSFXChanged);
		VolumeSFX->OnSliderValueChanged.AddDynamic(this, &USettingsPanel_Sound::ApplyVolumeSFXRuntime);
	}
	if (VolumeUI)
	{
		VolumeUI->OnSliderSaved.AddDynamic(this, &USettingsPanel_Sound::HandleVolumeUIChanged);
		VolumeUI->OnSliderValueChanged.AddDynamic(this, &USettingsPanel_Sound::ApplyVolumeUIRuntime);
	}
}

void USettingsPanel_Sound::NativeDestruct()
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

// 저장 이벤트
void USettingsPanel_Sound::HandleVolumeMasterChanged(float NewValue)
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->SetVolumeMaster(NewValue);
		}
	}
}

void USettingsPanel_Sound::HandleVolumeBGMChanged(float NewValue)
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->SetVolumeBGM(NewValue);
		}
	}
}

void USettingsPanel_Sound::HandleVolumeSFXChanged(float NewValue)
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->SetVolumeSFX(NewValue);
		}
	}
}

void USettingsPanel_Sound::HandleVolumeUIChanged(float NewValue)
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->SetVolumeUI(NewValue);
		}
	}
}

// 실시간 반영
void USettingsPanel_Sound::ApplyVolumeMasterRuntime(float NewValue)
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->ApplyVolumeMaster(NewValue);
		}
	}
}

void USettingsPanel_Sound::ApplyVolumeBGMRuntime(float NewValue)
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->ApplyVolumeBGM(NewValue);
		}
	}
}

void USettingsPanel_Sound::ApplyVolumeSFXRuntime(float NewValue)
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->ApplyVolumeSFX(NewValue);
		}
	}
}

void USettingsPanel_Sound::ApplyVolumeUIRuntime(float NewValue)
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->ApplyVolumeUI(NewValue);
		}
	}
}
