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
protected:
	void NativeConstruct();
	void NativeDestruct();
	
	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeMaster;

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeBGM;

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeSFX;

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeUI;
	

	UFUNCTION()
	void HandleVolumeMasterChanged(float NewValue);
	UFUNCTION()
	void HandleVolumeBGMChanged(float NewValue);
	UFUNCTION()
	void HandleVolumeSFXChanged(float NewValue);
	UFUNCTION()
	void HandleVolumeUIChanged(float NewValue);
};
