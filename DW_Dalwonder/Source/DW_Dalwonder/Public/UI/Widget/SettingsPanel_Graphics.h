// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsManager.h"
#include "SettingsPanel_Graphics.generated.h"

class UOptionSelectorWidget;
class USettingRowToggle;
class USettingRow_Slider;

UCLASS()
class DW_DALWONDER_API USettingsPanel_Graphics : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	USettingsManager* GetSettingsManager() const;
protected:
	/** 화면 모드 (전체화면/창모드 등) */
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UOptionSelectorWidget* WindowMode;

	/** 해상도 (1920x1080 등) */
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UOptionSelectorWidget* Resolution;

	/** 프레임 제한 (30, 60, 144 등) */
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UOptionSelectorWidget* FPSLimit;

	/** 밝기 (슬라이더) */
	// UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	// USettingRow_Slider* Brightness;

	/** VSync 토글 */
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	USettingRowToggle* VSync;

	/** 모션 블러 토글 */
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	USettingRowToggle* MotionBlur;

	/** 그림자 토글 */
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	USettingRowToggle* Shadows;

protected:
	UFUNCTION()
	void HandleWindowModeChanged(int32 SelectedIndex);

	UFUNCTION()
	void HandleResolutionChanged(int32 SelectedIndex);

	UFUNCTION()
	void HandleMotionBlurChanged(bool bOn);

	UFUNCTION()
	void HandleVSyncChanged(bool bOn);

	UFUNCTION()
	void HandleShadowsChanged(bool bOn);
	
	UFUNCTION()
	void HandleFPSLimitChanged(int32 Index);
	
	UFUNCTION()
	void InitResolutionSelector(USettingsManager* SM);
	
};
