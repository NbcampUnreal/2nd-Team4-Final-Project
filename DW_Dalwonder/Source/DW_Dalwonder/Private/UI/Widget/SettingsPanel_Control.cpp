#include "UI/Widget/SettingsPanel_Control.h"
#include "DW_GameInstance.h"
#include "Character/DW_PlayerController.h"
#include "UI/Widget/SettingsManager.h"
#include "UI/Widget/SettingRowSlider.h"
#include "UI/Widget/SettingRow_KeyBind.h"
#include "Components/Button.h"
#include "Components/Slider.h"

void USettingsPanel_Control::NativeConstruct()
{
	Super::NativeConstruct();

	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			// 1. 마우스 감도 초기화
			const float Saved = SM->GetMouseSensitivity();
			MouseSensitivity->Slider_Value->SetValue(Saved);
			MouseSensitivity->CurrentValue = Saved;
			MouseSensitivity->UpdateDisplayText(Saved);

			// 2. 키 바인딩 초기화
			auto SetupKeyBind = [SM](USettingRow_KeyBind* KeyBind, FName ActionName, const FString& Label)
			{
				if (!KeyBind)
				{
					UE_LOG(LogTemp, Warning, TEXT("[KeyBindSetup] Widget is null for %s"), *Label);
					return;
				}

				UE_LOG(LogTemp, Warning, TEXT("[KeyBindSetup] Initializing %s -> %s"), *Label, *ActionName.ToString());
				KeyBind->Init(ActionName, SM->GetKeyForAction(ActionName), FText::FromString(Label));
			};

			// 이동
			SetupKeyBind(KeyBind_MoveForward,   ADW_PlayerController::Action_MoveForward,  "Move Forward");
			SetupKeyBind(KeyBind_MoveBackward,  ADW_PlayerController::Action_MoveBackward, "Move Backward");
			SetupKeyBind(KeyBind_MoveLeft,      ADW_PlayerController::Action_MoveLeft,     "Move Left");
			SetupKeyBind(KeyBind_MoveRight,     ADW_PlayerController::Action_MoveRight,    "Move Right");

			// 기본 액션
			SetupKeyBind(KeyBind_Jump,      ADW_PlayerController::Action_Jump,      "Jump");
			SetupKeyBind(KeyBind_Interact,  ADW_PlayerController::Action_Interact,  "Interact");
			SetupKeyBind(KeyBind_Attack,    ADW_PlayerController::Action_Attack,    "Attack");
			SetupKeyBind(KeyBind_Guard,     ADW_PlayerController::Action_Guard,     "Guard");
			SetupKeyBind(KeyBind_Dodge,     ADW_PlayerController::Action_Dodge,     "Dodge");
			SetupKeyBind(KeyBind_Lockon,    ADW_PlayerController::Action_Lockon,    "Lockon");
			SetupKeyBind(KeyBind_Skill,     ADW_PlayerController::Action_Skill,     "Skill");

			// 스킬
			SetupKeyBind(KeyBind_UseSkill1, ADW_PlayerController::Action_Skill1, "Skill 1");
			SetupKeyBind(KeyBind_UseSkill2, ADW_PlayerController::Action_Skill2, "Skill 2");
			SetupKeyBind(KeyBind_UseSkill3, ADW_PlayerController::Action_Skill3, "Skill 3");

			// 기타
			SetupKeyBind(KeyBind_Ride, ADW_PlayerController::Action_Ride, "Ride");
		}
	}

	// 감도 바인딩
	MouseSensitivity->OnSliderSaved.AddDynamic(this, &USettingsPanel_Control::HandleMouseSensitivityChanged);

	// 키 바인딩 델리게이트
	auto BindDelegate = [this](USettingRow_KeyBind* KeyBind)
	{
		if (KeyBind)
			KeyBind->OnKeyAssigned.AddDynamic(this, &USettingsPanel_Control::HandleKeyBindChanged);
	};

	BindDelegate(KeyBind_MoveForward);
	BindDelegate(KeyBind_MoveBackward);
	BindDelegate(KeyBind_MoveLeft);
	BindDelegate(KeyBind_MoveRight);
	BindDelegate(KeyBind_Jump);
	BindDelegate(KeyBind_Attack);
	BindDelegate(KeyBind_Interact);
	BindDelegate(KeyBind_Guard);
	BindDelegate(KeyBind_Dodge);
	BindDelegate(KeyBind_Lockon);
	BindDelegate(KeyBind_Skill);
	BindDelegate(KeyBind_UseSkill1);
	BindDelegate(KeyBind_UseSkill2);
	BindDelegate(KeyBind_UseSkill3);
	BindDelegate(KeyBind_Ride);

	if (Button_ResetKeys)
	{
		Button_ResetKeys->OnClicked.AddDynamic(this, &USettingsPanel_Control::HandleResetKeyBindings);
	}
}

void USettingsPanel_Control::NativeDestruct()
{
	Super::NativeDestruct();
	UE_LOG(LogTemp, Warning, TEXT("SettingsPanel destructed, saving to slot"));

	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->SaveToSlot();
		}
	}
}

void USettingsPanel_Control::HandleMouseSensitivityChanged(float NewValue)
{
	const UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>();
	if (USettingsManager* SM = GI->GetSettingsManager())
	{
		SM->SetMouseSensitivity(NewValue);
	}
}

void USettingsPanel_Control::HandleKeyBindChanged(FName ActionName, FKey NewKey)
{
	UE_LOG(LogTemp, Warning, TEXT("[HandleKeyBindChanged] ActionName: %s | Key: %s"), *ActionName.ToString(), *NewKey.ToString());
	if (ActionName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[HandleKeyBindChanged] Ignored None ActionName"));
		return;
	}

	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->SetCustomKey(ActionName, NewKey);
		}
	}
}

void USettingsPanel_Control::HandleResetKeyBindings()
{
	if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			SM->ResetKeyBindingsToDefault();
			NativeConstruct();
		}
	}
}

void USettingsPanel_Control::RefreshKeyDisplayAll()
{
}
