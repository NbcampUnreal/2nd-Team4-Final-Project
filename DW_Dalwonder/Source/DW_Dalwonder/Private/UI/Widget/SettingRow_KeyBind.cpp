#include "UI/Widget/SettingRow_KeyBind.h"
#include "Components/TextBlock.h"
#include "Framework/Application/SlateApplication.h"

void USettingRow_KeyBind::NativeConstruct()
{
	Super::NativeConstruct();
	if (Text_KeyName)
	{
		Text_KeyName->SetText(FText::FromString(AssignedKey.GetDisplayName().ToString()));
	}
}

void USettingRow_KeyBind::StartListeningForKey()
{
	bIsListeningForInput = true;
	Text_KeyName->SetText(FText::FromString(TEXT("...")));
	SetKeyboardFocus();
}

FReply USettingRow_KeyBind::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (bIsListeningForInput)
	{
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