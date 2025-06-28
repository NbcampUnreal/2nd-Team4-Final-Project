#include "UI/Widget/SettingsManager.h"
#include "DW_SaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"

USettingsManager::USettingsManager()
{
	MasterMixAsset = TSoftObjectPtr<USoundMix>(FSoftObjectPath(TEXT("/Game/BluePrint/UI/Sounds/SoundClass/MasterMix.MasterMix")));
	MasterClassAsset = TSoftObjectPtr<USoundClass>(FSoftObjectPath(TEXT("/Game/BluePrint/UI/Sounds/SoundClass/SC_Master.SC_Master")));
	BGMClassAsset = TSoftObjectPtr<USoundClass>(FSoftObjectPath(TEXT("/Game/BluePrint/UI/Sounds/SoundClass/SC_BGM.SC_BGM")));
	SFXClassAsset = TSoftObjectPtr<USoundClass>(FSoftObjectPath(TEXT("/Game/BluePrint/UI/Sounds/SoundClass/SC_SFX.SC_SFX")));
	UIClassAsset = TSoftObjectPtr<USoundClass>(FSoftObjectPath(TEXT("/Game/BluePrint/UI/Sounds/SoundClass/SC_UI.SC_UI")));
}

void USettingsManager::Initialize()
{
	MasterMixAsset.LoadSynchronous();
	MasterClassAsset.LoadSynchronous();
	BGMClassAsset.LoadSynchronous();
	SFXClassAsset.LoadSynchronous();
	UIClassAsset.LoadSynchronous();

	MasterMix = MasterMixAsset.Get();
	MasterClass = MasterClassAsset.Get();
	BGMClass = BGMClassAsset.Get();
	SFXClass = SFXClassAsset.Get();
	UIClass = UIClassAsset.Get();

	UE_LOG(LogTemp, Warning, TEXT("MasterMix loaded? %s"), MasterMix ? TEXT("Yes") : TEXT("No"));

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

	UE_LOG(LogTemp, Warning, TEXT("[Save] Master: %.2f / BGM: %.2f / SFX: %.2f / UI: %.2f"),
		VolumeMaster, VolumeBGM, VolumeSFX, VolumeUI);
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

	WindowModeIndex = 0;
	ResolutionValue = FIntPoint(1920, 1080);
	FrameRateLimit = 60.f;
	bVSyncEnabled = false;
	bMotionBlurEnabled = true;
	bShadowEnabled = true;
}

void USettingsManager::LoadSettingsFrom(UDW_SaveGame* Save)
{
	UE_LOG(LogTemp, Warning, TEXT("[Load] Master: %.2f / BGM: %.2f / SFX: %.2f / UI: %.2f"),
		Save->SavedVolumeMaster, Save->SavedVolumeBGM, Save->SavedVolumeSFX, Save->SavedVolumeUI);

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
	ApplyVolumeMaster(Save->SavedVolumeMaster);
	ApplyVolumeBGM(Save->SavedVolumeBGM);
	ApplyVolumeSFX(Save->SavedVolumeSFX);
	ApplyVolumeUI(Save->SavedVolumeUI);

	UE_LOG(LogTemp, Warning, TEXT("[LoadSettingsFrom] ApplyVolumeMaster %.2f"), VolumeMaster);
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

	if (!MasterMix || !MasterClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ApplyVolumeMaster] Failed - MasterMix or MasterClass is null"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[ApplyVolumeMaster] Applying value: %.2f"), Value);

	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, MasterClass, Value / 100.f, 1.0f, 0.0f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::ApplyVolumeBGM(float Value)
{
	VolumeBGM = Value;

	if (!MasterMix || !BGMClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ApplyVolumeBGM] Failed - MasterMix or BGMClass is null"));
		return;
	}

	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, BGMClass, Value / 100.f, 1.0f, 0.0f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::ApplyVolumeSFX(float Value)
{
	VolumeSFX = Value;

	if (!MasterMix || !SFXClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ApplyVolumeSFX] Failed - MasterMix or SFXClass is null"));
		return;
	}

	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, SFXClass, Value / 100.f, 1.0f, 0.0f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::ApplyVolumeUI(float Value)
{
	VolumeUI = Value;

	if (!MasterMix || !UIClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ApplyVolumeUI] Failed - MasterMix or UIClass is null"));
		return;
	}

	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, UIClass, Value / 100.f, 1.0f, 0.0f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);

	UE_LOG(LogTemp, Warning, TEXT("[ApplyVolumeUI] Value = %.2f"), Value);
	UE_LOG(LogTemp, Warning, TEXT("[ApplyVolumeUI] MasterMix = %s"), MasterMix ? *MasterMix->GetName() : TEXT("nullptr"));
	UE_LOG(LogTemp, Warning, TEXT("[ApplyVolumeUI] UIClass = %s"), UIClass ? *UIClass->GetName() : TEXT("nullptr"));
}

void USettingsManager::SetVolumeMaster(float Value)
{
	VolumeMaster = Value;
}

void USettingsManager::SetVolumeBGM(float Value)
{
	VolumeBGM = Value;
}

void USettingsManager::SetVolumeSFX(float Value)
{
	VolumeSFX = Value;
}

void USettingsManager::SetVolumeUI(float Value)
{
	VolumeUI = Value;
}
