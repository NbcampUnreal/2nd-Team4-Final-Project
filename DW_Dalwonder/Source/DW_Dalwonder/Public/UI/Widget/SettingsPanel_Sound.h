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
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeMaster;

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeBGM;

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeSFX;

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* VolumeUI;

	// 저장 시 호출
	UFUNCTION()
	void HandleVolumeMasterChanged(float NewValue);
	UFUNCTION()
	void HandleVolumeBGMChanged(float NewValue);
	UFUNCTION()
	void HandleVolumeSFXChanged(float NewValue);
	UFUNCTION()
	void HandleVolumeUIChanged(float NewValue);

	// 실시간 반영용 (슬라이더 값 변화 시 호출)
	UFUNCTION()
	void ApplyVolumeMasterRuntime(float NewValue);
	UFUNCTION()
	void ApplyVolumeBGMRuntime(float NewValue);
	UFUNCTION()
	void ApplyVolumeSFXRuntime(float NewValue);
	UFUNCTION()
	void ApplyVolumeUIRuntime(float NewValue);
};
