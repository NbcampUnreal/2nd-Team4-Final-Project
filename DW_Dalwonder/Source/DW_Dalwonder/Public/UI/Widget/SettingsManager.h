#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "UObject/NoExportTypes.h"
#include "DW_SaveGame.h"
#include "InputAction.h"
#include "SettingsManager.generated.h"

/**
 * 게임 설정을 관리하는 객체
 * GameInstance에서 소유하며, UI 및 시스템 연동에 사용됨
 */
UCLASS()
class DW_DALWONDER_API USettingsManager : public UObject
{
	GENERATED_BODY()

public:
	// 생성자 및 초기화
	USettingsManager();
	void AutoInitializeInputActionMap();
	void Initialize();

	// 저장 & 불러오기
	void SaveToSlot();
	void SaveSettingsTo(UDW_SaveGame* Save);
	void LoadSettings();
	void LoadSettingsFrom(UDW_SaveGame* Save);
	void ApplySettings();

	// 그래픽 설정
	void ApplyWindowMode(int32 ModeIndex);
	void ApplyVSync(bool bEnable);
	void ApplyResolution(FIntPoint Resolution);
	void ApplyFrameLimit(float FPS);
	void ApplyMotionBlur(bool bEnable);
	void ApplyShadows(bool bEnable);

	// 오디오 설정
	void ApplyVolumeMaster(float Value);
	void ApplyVolumeBGM(float Value);
	void ApplyVolumeSFX(float Value);
	void ApplyVolumeUI(float Value);

	void SetVolumeMaster(float Value);
	void SetVolumeBGM(float Value);
	void SetVolumeSFX(float Value);
	void SetVolumeUI(float Value);

	// 컨트롤 설정
	void ApplyMouseSensitivity(float InSensitivity);
	void SetMouseSensitivity(float NewSensitivity);
	float GetMouseSensitivity() const;

	void SetCustomKey(FName ActionName, FKey NewKey);
	FKey GetKeyForAction(FName ActionName) const;
	FKey GetDefaultKeyForAction(FName ActionName) const;
	TMap<FName, FKey> GetDefaultKeyMap() const;
	void ResetKeyBindingsToDefault();
	void ApplyKeyBindingsToInputSystem();
	UInputAction* GetInputActionByName(FName ActionName) const;
	
	/** Enhanced Input 매핑을 위한 컨텍스트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	/** 액션 이름을 키로 하는 InputAction 맵 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TMap<FName, UInputAction*> InputActionMap;

	// Getter 함수들
	int32 GetWindowModeIndex() const;
	FIntPoint GetResolution() const;
	float GetFrameRateLimit() const;
	bool IsVSyncEnabled() const;
	bool IsMotionBlurEnabled() const;
	bool IsShadowEnabled() const;
	void DebugPrintAllMappings();

	float GetVolumeMaster() const;
	float GetVolumeBGM() const;
	float GetVolumeSFX() const;
	float GetVolumeUI() const;
	USoundMix* GetMasterMix() const;

	TMap<FName, FKey> GetCustomKeyMap() const;

protected:
	// 에셋 (디자인 타임에 할당 가능)
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
	// 로드된 사운드 클래스 및 믹스 (런타임에만 필요)
	UPROPERTY()
	USoundMix* MasterMix = nullptr;

	UPROPERTY()
	USoundClass* MasterClass = nullptr;

	UPROPERTY()
	USoundClass* BGMClass = nullptr;

	UPROPERTY()
	USoundClass* SFXClass = nullptr;

	UPROPERTY()
	USoundClass* UIClass = nullptr;

	// 설정값 저장용 내부 변수
	int32 WindowModeIndex = 0;
	FIntPoint ResolutionValue = { 1920, 1080 };
	float FrameRateLimit = 60.0f;
	bool bVSyncEnabled = false;
	bool bMotionBlurEnabled = true;
	bool bShadowEnabled = true;

	// 컨트롤
	float MouseSensitivity = 1.0f;

	// 오디오
	float VolumeMaster = 100.f;
	float VolumeBGM = 100.f;
	float VolumeSFX = 100.f;
	float VolumeUI = 100.f;

	// 키 매핑
	UPROPERTY()
	TMap<FName, FKey> CustomKeyMap;
};
