#include "UI/Widget/SettingsManager.h"
#include "DW_SaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void USettingsManager::Initialize()
{
	LoadSettings();
}

void USettingsManager::ApplySettings()
{
	ApplyWindowMode(WindowModeIndex);
	ApplyVSync(bVSyncEnabled);
	ApplyResolution(ResolutionValue);
	ApplyFrameLimit(FrameRateLimit);
	ApplyMotionBlur(bMotionBlurEnabled);
	ApplyShadows(bShadowEnabled);

	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->ApplySettings(false);
	}
}

void USettingsManager::SaveSettingsTo(UDW_SaveGame* Save)
{
	Save->SavedWindowMode = WindowModeIndex;
	Save->SavedResolution = ResolutionValue;
	Save->bSavedVSync = bVSyncEnabled;
	Save->SavedFrameRate = FrameRateLimit;
	Save->bSavedMotionBlur = bMotionBlurEnabled;
	Save->bSavedShadow = bShadowEnabled;
	Save->SavedMouseSensitivity = MouseSensitivity;
	Save->SavedVolumeMaster = VolumeMaster;
	Save->SavedVolumeBGM = VolumeBGM;
	Save->SavedVolumeSFX = VolumeSFX;
	Save->SavedVolumeUI = VolumeUI;
}

void USettingsManager::SaveToSlot()
{
	UDW_SaveGame* Save = Cast<UDW_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDW_SaveGame::StaticClass()));
	if (!Save) return;

	SaveSettingsTo(Save);
	UGameplayStatics::SaveGameToSlot(Save, TEXT("Default"), 0);
}

void USettingsManager::LoadSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Default"), 0))
	{
		UDW_SaveGame* Save = Cast<UDW_SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Default"), 0));
		if (Save)
		{
			LoadSettingsFrom(Save);
			return;
		}
	}

	// 기본값으로 초기화
	WindowModeIndex = 0;
	ResolutionValue = FIntPoint(1920, 1080);
	FrameRateLimit = 60.f;
	bVSyncEnabled = false;
	bMotionBlurEnabled = true;
	bShadowEnabled = true;
}

void USettingsManager::LoadSettingsFrom(UDW_SaveGame* Save)
{
	ApplyWindowMode(Save->SavedWindowMode);
	ApplyResolution(Save->SavedResolution);
	ApplyVSync(Save->bSavedVSync);
	ApplyFrameLimit(Save->SavedFrameRate);
	ApplyMotionBlur(Save->bSavedMotionBlur);
	ApplyShadows(Save->bSavedShadow);
	MouseSensitivity = Save->SavedMouseSensitivity;
	SetVolumeMaster(Save->SavedVolumeMaster);
	SetVolumeBGM(Save->SavedVolumeBGM);
	SetVolumeSFX(Save->SavedVolumeSFX);
	SetVolumeUI(Save->SavedVolumeUI);
}

void USettingsManager::ApplyWindowMode(int32 ModeIndex)
{
	WindowModeIndex = ModeIndex;

	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		EWindowMode::Type Mode = EWindowMode::Fullscreen;
		switch (ModeIndex)
		{
		case 0: Mode = EWindowMode::Fullscreen; break;
		case 1: Mode = EWindowMode::WindowedFullscreen; break;
		case 2: Mode = EWindowMode::Windowed; break;
		}
		Settings->SetFullscreenMode(Mode);
		Settings->ApplySettings(false);
	}
}

void USettingsManager::ApplyVSync(bool bEnable)
{
	bVSyncEnabled = bEnable;
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.VSync"));
	if (CVar)
	{
		CVar->Set(bEnable ? 1 : 0);
	}
}

void USettingsManager::ApplyResolution(FIntPoint InResolution)
{
	ResolutionValue = InResolution;
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetScreenResolution(InResolution);
		Settings->ApplySettings(false);
	}
}

void USettingsManager::ApplyFrameLimit(float FPS)
{
	FrameRateLimit = FPS;
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetFrameRateLimit(FPS);
		Settings->ApplySettings(false);
	}
}

void USettingsManager::ApplyMotionBlur(bool bEnable)
{
	bMotionBlurEnabled = bEnable;
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MotionBlurQuality"));
	if (CVar)
	{
		CVar->Set(bEnable ? 1 : 0);
	}
}

void USettingsManager::ApplyShadows(bool bEnable)
{
	bShadowEnabled = bEnable;
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.ShadowQuality"));
	if (CVar)
	{
		CVar->Set(bEnable ? 3 : 0);
	}
}

void USettingsManager::ApplyMouseSensitivity(float InSensitivity)
{
	MouseSensitivity = InSensitivity;

}

void USettingsManager::ApplyVolumeMaster(float Value)
{
	VolumeMaster = Value;
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, MasterClass, Value / 100.f, 1.0f, 0.0f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::ApplyVolumeBGM(float Value)
{
	VolumeBGM = Value;
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, BGMClass, Value / 100.f, 1.0f, 0.0f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::ApplyVolumeSFX(float Value)
{
	VolumeSFX = Value;
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, SFXClass, Value / 100.f, 1.0f, 0.0f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::ApplyVolumeUI(float Value)
{
	VolumeUI = Value;
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, UIClass, Value / 100.f, 1.0f, 0.0f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::SetVolumeMaster(float Value)
{
	VolumeMaster = Value;
	// 여기에 사운드 시스템 연동 코드 추가 예정
}

void USettingsManager::SetVolumeBGM(float Value)
{
	VolumeBGM = Value;
	// 여기에 BGM 사운드 믹스 적용 코드 추가 예정
}

void USettingsManager::SetVolumeSFX(float Value)
{
	VolumeSFX = Value;
	// 여기에 효과음 볼륨 반영 코드 추가 예정
}

void USettingsManager::SetVolumeUI(float Value)
{
	VolumeUI = Value;
	// 여기에 UI 사운드 볼륨 반영 코드 추가 예정
}