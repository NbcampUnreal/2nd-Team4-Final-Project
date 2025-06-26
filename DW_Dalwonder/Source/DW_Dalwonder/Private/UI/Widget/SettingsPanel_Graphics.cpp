#include "UI/Widget/SettingsPanel_Graphics.h"
#include "UI/Widget/OptionSelectorWidget.h"
#include "UI/Widget/SettingsManager.h"
#include "UI/Widget/SettingRowToggle.h"
#include "DW_GameInstance.h"

static bool ParseResolution(const FString& InStr, FVector2D& OutSize)
{
	FString Left, Right;
	if (InStr.Split(TEXT("x"), &Left, &Right))
	{
		OutSize.X = FCString::Atoi(*Left);
		OutSize.Y = FCString::Atoi(*Right);
		return true;
	}
	return false;
}

USettingsManager* USettingsPanel_Graphics::GetSettingsManager() const
{
	if (const UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is nullptr!"));
		return GI->GetSettingsManager();
	}
	return nullptr;
}

void USettingsPanel_Graphics::NativeConstruct()
{
	Super::NativeConstruct();

	USettingsManager* SM = GetSettingsManager();
	if (!SM) return;

	if (WindowMode)
	{
		WindowMode->SetSelectedIndex(SM->GetWindowModeIndex());
		WindowMode->OnOptionSelected.AddDynamic(this, &USettingsPanel_Graphics::HandleWindowModeChanged);
	}

	InitResolutionSelector(SM);

	if (MotionBlur)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetToggleState(MotionBlur): %d"), SM->IsMotionBlurEnabled());
		MotionBlur->SetToggleState(SM->IsMotionBlurEnabled());
		MotionBlur->OnToggleChanged.AddDynamic(this, &USettingsPanel_Graphics::HandleMotionBlurChanged);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MotionBlur is null"));
	}

	if (FPSLimit)
	{
		float FPS = SM->GetFrameRateLimit();
		int32 Index = 0;
		if (FPS == 30) Index = 0;
		else if (FPS == 60) Index = 1;
		else if (FPS == 144) Index = 2;
		else if (FPS == 256) Index = 3;
		else Index = 4;

		FPSLimit->SetSelectedIndex(Index);
		FPSLimit->OnOptionSelected.AddDynamic(this, &USettingsPanel_Graphics::HandleFPSLimitChanged);
	}

	if (VSync)
	{
		VSync->SetToggleState(SM->IsVSyncEnabled());
		VSync->OnToggleChanged.AddDynamic(this, &USettingsPanel_Graphics::HandleVSyncChanged);
	}

	if (Shadows)
	{
		Shadows->SetToggleState(SM->IsShadowEnabled());
		Shadows->OnToggleChanged.AddDynamic(this, &USettingsPanel_Graphics::HandleShadowsChanged);
	}
}

void USettingsPanel_Graphics::NativeDestruct()
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

void USettingsPanel_Graphics::InitResolutionSelector(USettingsManager* SM)
{
	if (!Resolution || !SM) return;

	FIntPoint SavedRes = SM->GetResolution();
	const TArray<FText>& OptionList = Resolution->GetOptionList();

	for (int32 i = 0; i < OptionList.Num(); ++i)
	{
		FVector2D Parsed;
		if (ParseResolution(OptionList[i].ToString(), Parsed))
		{
			if (FMath::IsNearlyEqual(Parsed.X, SavedRes.X) && FMath::IsNearlyEqual(Parsed.Y, SavedRes.Y))
			{
				Resolution->SetSelectedIndex(i);
				break;
			}
		}
	}

	Resolution->OnOptionSelected.AddDynamic(this, &USettingsPanel_Graphics::HandleResolutionChanged);
}

void USettingsPanel_Graphics::HandleWindowModeChanged(int32 SelectedIndex)
{
	if (USettingsManager* SM = GetSettingsManager())
	{
		SM->ApplyWindowMode(SelectedIndex);
	}
}

void USettingsPanel_Graphics::HandleResolutionChanged(int32 SelectedIndex)
{
	if (USettingsManager* SM = GetSettingsManager())
	{
		FString ResStr = Resolution->GetSelectedOption();
		FVector2D ParsedSize;
		if (ParseResolution(ResStr, ParsedSize))
		{
			SM->ApplyResolution(FIntPoint(ParsedSize.X, ParsedSize.Y));
		}
	}
}

void USettingsPanel_Graphics::HandleMotionBlurChanged(bool bOn)
{
	if (USettingsManager* SM = GetSettingsManager())
	{
		SM->ApplyMotionBlur(bOn);
	}
}

void USettingsPanel_Graphics::HandleVSyncChanged(bool bOn)
{
	if (USettingsManager* SM = GetSettingsManager())
	{
		SM->ApplyVSync(bOn);
	}
}

void USettingsPanel_Graphics::HandleShadowsChanged(bool bOn)
{
	if (USettingsManager* SM = GetSettingsManager())
	{
		SM->ApplyShadows(bOn);
	}
}

void USettingsPanel_Graphics::HandleFPSLimitChanged(int32 Index)
{
	if (USettingsManager* SM = GetSettingsManager())
	{
		switch (Index)
		{
		case 0: SM->ApplyFrameLimit(30); break;
		case 1: SM->ApplyFrameLimit(60); break;
		case 2: SM->ApplyFrameLimit(144); break;
		case 3: SM->ApplyFrameLimit(256); break;
		case 4: SM->ApplyFrameLimit(0.f); break;
		default: break;
		}
	}
}
