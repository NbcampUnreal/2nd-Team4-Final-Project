// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/SettingRowSlider.h"
#include "SettingsPanel_Control.generated.h"

/**
 * 
 */
class UButton;

UCLASS()
class DW_DALWONDER_API USettingsPanel_Control : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	void NativeDestruct();

	UFUNCTION()
	void HandleMouseSensitivityChanged(float NewValue);
	
	UFUNCTION()
	void HandleKeyBindChanged(FName ActionName, FKey NewKey);
	
	UFUNCTION()
	void HandleResetKeyBindings();
	UFUNCTION()
	void RefreshKeyDisplayAll();

	UPROPERTY(meta = (BindWidget))
	USettingRowSlider* MouseSensitivity;
	
	// 키 바인딩 위젯 참조 (BindWidget)
	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_LookUp;
	
	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_Turn;
	
	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_MoveForward;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_MoveBackward;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_MoveLeft;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_MoveRight;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_Jump;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_Attack;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_Interact;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_Guard;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_Dodge;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_Lockon;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_Skill;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_UseSkill1;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_UseSkill2;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_UseSkill3;

	UPROPERTY(meta = (BindWidget))
	class USettingRow_KeyBind* KeyBind_Ride;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_ResetKeys;
};
