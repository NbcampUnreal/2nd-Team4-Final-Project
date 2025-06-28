// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_SaveGame.h"
#include "UObject/NoExportTypes.h"
#include "SettingsManager.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API USettingsManager : public UObject
{
	GENERATED_BODY()
public:
	USettingsManager();
	/** 초기화 (GameInstance에서 호출) */
	void Initialize();

	/** 현재 설정값 적용 */
	void ApplySettings();

	/** 설정 저장 */
	void SaveSettingsTo(UDW_SaveGame* Save);

	/** 설정 로드 */
	void LoadSettings();
	void LoadSettingsFrom(UDW_SaveGame* Save);

	/** 개별 적용 함수 예시 */
	void ApplyWindowMode(int32 ModeIndex);
	void ApplyVSync(bool bEnable);
	void ApplyResolution(FIntPoint Resolution);
	void ApplyFrameLimit(float FPS);
	void ApplyMotionBlur(bool bEnable);
	void ApplyShadows(bool bEnable);
	void ApplyMouseSensitivity(float InSensitivity);
	void ApplyVolumeMaster(float Value);
	void ApplyVolumeBGM(float Value);
	void ApplyVolumeSFX(float Value);
	void ApplyVolumeUI(float Value);
	void SaveToSlot();

	USoundMix* MasterMix = nullptr;
	USoundClass* MasterClass = nullptr;
	USoundClass* BGMClass = nullptr;
	USoundClass* SFXClass = nullptr;
	USoundClass* UIClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundMix> MasterMixAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundClass> MasterClassAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundClass> BGMClassAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundClass> SFXClassAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundClass> UIClassAsset;

private:
	/** 저장용 내부 변수들 */
	int32 WindowModeIndex;
	FIntPoint ResolutionValue;

	float MouseSensitivity = 1.0f;
	float VolumeMaster = 100.f;
	float VolumeBGM = 100.f;
	float VolumeSFX = 100.f;
	float VolumeUI = 100.f;
	float FrameRateLimit;
	bool bVSyncEnabled;
	bool bMotionBlurEnabled;
	bool bShadowEnabled;

	
public:
	// 그래픽
    int32 GetWindowModeIndex() const { return WindowModeIndex; }
    FIntPoint GetResolution() const { return ResolutionValue; }
	float GetFrameRateLimit() const { return FrameRateLimit; }
    bool IsVSyncEnabled() const { return bVSyncEnabled; }
    bool IsMotionBlurEnabled() const { return bMotionBlurEnabled; }
    bool IsShadowEnabled() const { return bShadowEnabled; }
	// 컨트롤러
	float GetMouseSensitivity() const { return MouseSensitivity; }
	void SetMouseSensitivity(float NewSensitivity) { MouseSensitivity = NewSensitivity; }
	// 사운드
	float GetVolumeMaster() const { return VolumeMaster; }
	float GetVolumeBGM() const { return VolumeBGM; }
	float GetVolumeSFX() const { return VolumeSFX; }
	float GetVolumeUI() const { return VolumeUI; }
	void SetVolumeMaster(float Value);
	void SetVolumeBGM(float Value);
	void SetVolumeSFX(float Value);
	void SetVolumeUI(float Value);

};
