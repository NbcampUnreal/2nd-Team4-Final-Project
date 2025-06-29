#include "UI/Widget/SettingRow_KeyBind.h"

#include "DW_GameInstance.h"
#include "Components/TextBlock.h"
#include "Framework/Application/SlateApplication.h"
#include "Components/Button.h"
#include "UI/Widget/SettingsManager.h"

void USettingRow_KeyBind::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (Text_KeyName)
	{
		Text_KeyName->SetText(FText::FromString(AssignedKey.GetDisplayName().ToString()));
	}

	if (UTextBlock* Label = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_SettingLabel"))))
	{
		Label->SetText(LabelText);
	}
}

void USettingRow_KeyBind::NativeConstruct()
{
	Super::NativeConstruct();
	
	UE_LOG(LogTemp, Warning, TEXT("[KeyBindConstruct] Widget: %s | ActionName: %s"), *GetName(), *ActionName.ToString());

	SetIsFocusable(true);
	
	if (Text_KeyName)
	{
		Text_KeyName->SetText(FText::FromString(AssignedKey.GetDisplayName().ToString()));
	}
	if (Button_Reset)
	{
		Button_Reset->OnClicked.AddDynamic(this, &USettingRow_KeyBind::OnResetButtonClicked);
	}
	if (Button_AssignKey)
	{
		Button_AssignKey->OnClicked.AddDynamic(this, &USettingRow_KeyBind::StartListeningForKey);
	}

	UE_LOG(LogTemp, Warning, TEXT("NativeConstruct - ActionName: %s"), *ActionName.ToString());
}

void USettingRow_KeyBind::StartListeningForKey()
{
	if (ActionName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[StartListening] Blocked - ActionName is None"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[StartListening] ActionName: %s"), *ActionName.ToString());

	bIsListeningForInput = true;
	Text_KeyName->SetText(FText::FromString(TEXT("...")));
	SetKeyboardFocus();
}

FReply USettingRow_KeyBind::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (bIsListeningForInput)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pressed key: %s"), *InKeyEvent.GetKey().ToString());
		bIsListeningForInput = false;

		const FKey PressedKey = InKeyEvent.GetKey();
		AssignedKey = PressedKey;

		if (Text_KeyName)
		{
			Text_KeyName->SetText(FText::FromString(PressedKey.GetDisplayName().ToString()));
		}

		OnKeyAssigned.Broadcast(ActionName, PressedKey);

		// 입력 먹힘 방지용
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void USettingRow_KeyBind::RefreshKeyDisplay()
{
	if (Text_KeyName)
	{
		Text_KeyName->SetText(AssignedKey.GetDisplayName());
	}
}

void USettingRow_KeyBind::HandleResetToDefault()
{
	if (const UWorld* World = GetWorld())
	{
		if (const UDW_GameInstance* GI = World->GetGameInstance<UDW_GameInstance>())
		{
			if (USettingsManager* SM = GI->GetSettingsManager())
			{
				const FKey DefaultKey = SM->GetDefaultKeyForAction(ActionName);
				AssignedKey = DefaultKey;
				SM->SetCustomKey(ActionName, DefaultKey);
				RefreshKeyDisplay();
			}
		}
	}
}

void USettingRow_KeyBind::OnResetButtonClicked()
{
	if (const UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
	{
		if (USettingsManager* SM = GI->GetSettingsManager())
		{
			const FKey DefaultKey = SM->GetDefaultKeyForAction(ActionName); // 아래 참고
			AssignedKey = DefaultKey;

			// UI 갱신
			RefreshKeyDisplay();

			// 시스템에 반영
			SM->SetCustomKey(ActionName, AssignedKey);

			// 델리게이트 알림 (옵션)
			OnKeyAssigned.Broadcast(ActionName, AssignedKey);
		}
	}
}

void USettingRow_KeyBind::Init(FName InActionName, FKey InAssignedKey, FText InLabelText)
{
	ActionName = InActionName;
	AssignedKey = InAssignedKey;
	LabelText = InLabelText;
	RefreshKeyDisplay();
}
