#include "UI/Widget/SettingsManager.h"
#include "DW_SaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "AudioDevice.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Character/DW_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

USettingsManager::USettingsManager()
{
	MasterMixAsset = TSoftObjectPtr<USoundMix>(FSoftObjectPath(TEXT("/Game/BluePrint/UI/Sounds/SoundClass/MasterMix.MasterMix")));
	MasterClassAsset = TSoftObjectPtr<USoundClass>(FSoftObjectPath(TEXT("/Game/BluePrint/UI/Sounds/SoundClass/SC_Master.SC_Master")));
	BGMClassAsset = TSoftObjectPtr<USoundClass>(FSoftObjectPath(TEXT("/Game/BluePrint/UI/Sounds/SoundClass/SC_BGM.SC_BGM")));
	SFXClassAsset = TSoftObjectPtr<USoundClass>(FSoftObjectPath(TEXT("/Game/BluePrint/UI/Sounds/SoundClass/SC_SFX.SC_SFX")));
	UIClassAsset   = TSoftObjectPtr<USoundClass>(FSoftObjectPath(TEXT("/Game/BluePrint/UI/Sounds/SoundClass/SC_UI.SC_UI")));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("/Game/Input/IMC_Default.IMC_Default"));
	if (IMC.Succeeded())
	{
		DefaultMappingContext = IMC.Object;
	}
}

void USettingsManager::Initialize()
{
	// 에셋 로드
	MasterMix = MasterMixAsset.LoadSynchronous();
	MasterClass = MasterClassAsset.LoadSynchronous();
	BGMClass = BGMClassAsset.LoadSynchronous();
	SFXClass = SFXClassAsset.LoadSynchronous();
	UIClass = UIClassAsset.LoadSynchronous();

	// 저장값 로드 또는 기본값 설정
	LoadSettings();

	if (CustomKeyMap.Num() == 0)
	{
		CustomKeyMap = GetDefaultKeyMap();
	}

	// 오디오 즉시 반영
	ApplyVolumeMaster(VolumeMaster);
	ApplyVolumeBGM(VolumeBGM);
	ApplyVolumeSFX(VolumeSFX);
	ApplyVolumeUI(VolumeUI);
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

void USettingsManager::SaveToSlot()
{
	if (UDW_SaveGame* Save = Cast<UDW_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDW_SaveGame::StaticClass())))
	{
		SaveSettingsTo(Save);
		for (const auto& Pair : CustomKeyMap)
		{
			if (Pair.Key.IsNone())
			{
				UE_LOG(LogTemp, Warning, TEXT("[SettingsManager] Skipping save for None key: %s"), *Pair.Value.ToString());
				continue;
			}
			UE_LOG(LogTemp, Warning, TEXT("Saving KeyMap - %s: %s"), *Pair.Key.ToString(), *Pair.Value.ToString());
		}
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Default"), 0);
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
	Save->SavedKeyMap = CustomKeyMap;
}

void USettingsManager::LoadSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Default"), 0))
	{
		if (UDW_SaveGame* Save = Cast<UDW_SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Default"), 0)))
		{
			LoadSettingsFrom(Save);
			return;
		}
	}

	// 기본값
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

	ApplyVolumeMaster(Save->SavedVolumeMaster);
	ApplyVolumeBGM(Save->SavedVolumeBGM);
	ApplyVolumeSFX(Save->SavedVolumeSFX);
	ApplyVolumeUI(Save->SavedVolumeUI);

	CustomKeyMap = Save->SavedKeyMap;
}

// ===== 그래픽 설정 =====

void USettingsManager::ApplyWindowMode(int32 ModeIndex)
{
	WindowModeIndex = ModeIndex;

	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		EWindowMode::Type Mode = EWindowMode::Fullscreen;
		switch (ModeIndex)
		{
		case 1: Mode = EWindowMode::WindowedFullscreen; break;
		case 2: Mode = EWindowMode::Windowed; break;
		}
		Settings->SetFullscreenMode(Mode);
	}
}

void USettingsManager::ApplyVSync(bool bEnable)
{
	bVSyncEnabled = bEnable;
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.VSync")))
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
	}
}

void USettingsManager::ApplyFrameLimit(float FPS)
{
	FrameRateLimit = FPS;
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetFrameRateLimit(FPS);
	}
}

void USettingsManager::ApplyMotionBlur(bool bEnable)
{
	bMotionBlurEnabled = bEnable;
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MotionBlurQuality")))
	{
		CVar->Set(bEnable ? 1 : 0);
	}
}

void USettingsManager::ApplyShadows(bool bEnable)
{
	bShadowEnabled = bEnable;
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.ShadowQuality")))
	{
		CVar->Set(bEnable ? 3 : 0);
	}
}

// ===== 오디오 설정 =====

void USettingsManager::ApplyVolumeMaster(float Value)
{
	VolumeMaster = Value;
	UGameplayStatics::ClearSoundMixModifiers(this);
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, MasterClass, Value / 100.f, 1.f);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::ApplyVolumeBGM(float Value)
{
	VolumeBGM = Value;
	if (!MasterMix || !BGMClass) return;
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, BGMClass, Value / 100.f, 1.f, 0.f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::ApplyVolumeSFX(float Value)
{
	VolumeSFX = Value;
	if (!MasterMix || !SFXClass) return;
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, SFXClass, Value / 100.f, 1.f, 0.f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::ApplyVolumeUI(float Value)
{
	VolumeUI = Value;
	if (!MasterMix || !UIClass) return;
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, UIClass, Value / 100.f, 1.f, 0.f, true);
	UGameplayStatics::PushSoundMixModifier(this, MasterMix);
}

void USettingsManager::SetVolumeMaster(float Value) { VolumeMaster = Value; }
void USettingsManager::SetVolumeBGM(float Value)    { VolumeBGM = Value; }
void USettingsManager::SetVolumeSFX(float Value)    { VolumeSFX = Value; }
void USettingsManager::SetVolumeUI(float Value)     { VolumeUI = Value; }

// ===== 컨트롤 설정 =====

void USettingsManager::ApplyMouseSensitivity(float InSensitivity)
{
	MouseSensitivity = InSensitivity;
}

float USettingsManager::GetMouseSensitivity() const { return MouseSensitivity; }

void USettingsManager::SetMouseSensitivity(float NewSensitivity)
{
	MouseSensitivity = NewSensitivity;
}

void USettingsManager::SetCustomKey(FName ActionName, FKey NewKey)
{
	if (ActionName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[SettingsManager] Ignored SetCustomKey with None ActionName, Key: %s"), *NewKey.ToString());
		return;
	}
	CustomKeyMap.FindOrAdd(ActionName) = NewKey;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (ADW_PlayerController* DWPC = Cast<ADW_PlayerController>(PC))
		{
			DWPC->ApplyCustomKeyBindings(CustomKeyMap);
		}
	}
}

FKey USettingsManager::GetKeyForAction(FName ActionName) const
{
	if (const FKey* Found = CustomKeyMap.Find(ActionName))
	{
		return *Found;
	}
	return EKeys::Invalid;
}

FKey USettingsManager::GetDefaultKeyForAction(FName ActionName) const
{
	const TMap<FName, FKey> DefaultMap = GetDefaultKeyMap();
	if (const FKey* Found = DefaultMap.Find(ActionName))
	{
		return *Found;
	}
	return EKeys::Invalid;
}

TMap<FName, FKey> USettingsManager::GetDefaultKeyMap() const
{
	TMap<FName, FKey> DefaultMap;

	DefaultMap.Add(ADW_PlayerController::Action_Jump, EKeys::SpaceBar);
	DefaultMap.Add(ADW_PlayerController::Action_Interact, EKeys::E);
	DefaultMap.Add(ADW_PlayerController::Action_Attack, EKeys::LeftMouseButton);
	DefaultMap.Add(ADW_PlayerController::Action_Guard, EKeys::RightMouseButton);
	DefaultMap.Add(ADW_PlayerController::Action_ESC, EKeys::Zero);
	DefaultMap.Add(ADW_PlayerController::Action_Lockon, EKeys::MiddleMouseButton);
	DefaultMap.Add(ADW_PlayerController::Action_Dodge, EKeys::LeftShift);
	DefaultMap.Add(ADW_PlayerController::Action_Skill, EKeys::R);
	DefaultMap.Add(ADW_PlayerController::Action_Skill1, EKeys::One);
	DefaultMap.Add(ADW_PlayerController::Action_Skill2, EKeys::Two);
	DefaultMap.Add(ADW_PlayerController::Action_Skill3, EKeys::Three);
	DefaultMap.Add(ADW_PlayerController::Action_Ride, EKeys::G);
	DefaultMap.Add(ADW_PlayerController::Action_MoveForward, EKeys::W);
	DefaultMap.Add(ADW_PlayerController::Action_MoveBackward, EKeys::S);
	DefaultMap.Add(ADW_PlayerController::Action_MoveLeft, EKeys::A);
	DefaultMap.Add(ADW_PlayerController::Action_MoveRight, EKeys::D);

	return DefaultMap;
}

void USettingsManager::ResetKeyBindingsToDefault()
{
	CustomKeyMap = GetDefaultKeyMap();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (ADW_PlayerController* DWPC = Cast<ADW_PlayerController>(PC))
		{
			DWPC->ApplyCustomKeyBindings(CustomKeyMap);
		}
	}

	SaveToSlot();
}

void USettingsManager::ApplyKeyBindingsToInputSystem()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->ClearAllMappings();

				for (const TPair<FName, FKey>& Pair : CustomKeyMap)
				{
					if (Pair.Key.IsNone() || !Pair.Value.IsValid())
						continue;

					if (UInputAction* InputAction = GetInputActionByName(Pair.Key))
					{
						// 예시 Context: 반드시 유효한 MappingContext를 먼저 지정해야 함
						if (UInputMappingContext* MappingContext = DefaultMappingContext)
						{
							Subsystem->AddPlayerMappedKey(Pair.Key, Pair.Value, FModifyContextOptions());
							UE_LOG(LogTemp, Warning, TEXT("[EnhancedInput] %s -> %s"), *Pair.Key.ToString(), *Pair.Value.ToString());
						}
					}
				}
			}
		}
	}
}

UInputAction* USettingsManager::GetInputActionByName(FName ActionName) const
{
	if (InputActionMap.Contains(ActionName))
	{
		return InputActionMap[ActionName];
	}
	return nullptr;
}


float USettingsManager::GetVolumeMaster() const { return VolumeMaster; }
float USettingsManager::GetVolumeBGM() const { return VolumeBGM; }
float USettingsManager::GetVolumeSFX() const { return VolumeSFX; }
float USettingsManager::GetVolumeUI() const { return VolumeUI; }

int32 USettingsManager::GetWindowModeIndex() const { return WindowModeIndex; }
FIntPoint USettingsManager::GetResolution() const { return ResolutionValue; }
float USettingsManager::GetFrameRateLimit() const { return FrameRateLimit; }

bool USettingsManager::IsVSyncEnabled() const { return bVSyncEnabled; }
bool USettingsManager::IsMotionBlurEnabled() const { return bMotionBlurEnabled; }
bool USettingsManager::IsShadowEnabled() const { return bShadowEnabled; }