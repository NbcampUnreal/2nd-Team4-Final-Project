// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingRowSlider.h"
#include "SettingsPanel_Sound.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API USettingsPanel_Sound : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeMaster;

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeBGM;

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeSFX;

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeUI;
	
	void NativeConstruct();
	void NativeDestruct();
	void HandleVolumeMasterChanged(float NewValue);
	void HandleVolumeBGMChanged(float NewValue);
	void HandleVolumeSFXChanged(float NewValue);
	void HandleVolumeUIChanged(float NewValue);
};
