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
	void SaveToSlot();

private:
	/** 저장용 내부 변수들 */
	int32 WindowModeIndex;
	FIntPoint ResolutionValue;

	float FrameRateLimit;
	bool bVSyncEnabled;
	bool bMotionBlurEnabled;
	bool bShadowEnabled;

	
public:
    int32 GetWindowModeIndex() const { return WindowModeIndex; }
    FIntPoint GetResolution() const { return ResolutionValue; }
	float GetFrameRateLimit() const { return FrameRateLimit; }
    bool IsVSyncEnabled() const { return bVSyncEnabled; }
    bool IsMotionBlurEnabled() const { return bMotionBlurEnabled; }
    bool IsShadowEnabled() const { return bShadowEnabled; }
};
