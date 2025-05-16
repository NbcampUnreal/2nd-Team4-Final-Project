#include "UI/Widget/CustomButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

void UCustomButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonMain)
	{
		ButtonMain->OnClicked.AddDynamic(this, &UCustomButtonWidget::HandleClicked);
		ButtonMain->OnHovered.AddDynamic(this, &UCustomButtonWidget::HandleHovered);
	}

	ApplyStyle();
	ApplyText();
}

void UCustomButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// 디자인 타임에서도 텍스트와 스타일 적용
	ApplyStyle();
	ApplyText();
}


void UCustomButtonWidget::HandleClicked()
{
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}
	OnCustomClicked.Broadcast();
}

void UCustomButtonWidget::HandleHovered()
{
	if (HoverSound)
	{
		UGameplayStatics::PlaySound2D(this, HoverSound);
	}
}

void UCustomButtonWidget::ApplyStyle()
{
	if (!ButtonMain) return;

	FButtonStyle Style;

	// 이미지 스타일
	if (NormalStyle.DrawAs != ESlateBrushDrawType::NoDrawType)
		Style.Normal = NormalStyle;
	if (HoveredStyle.DrawAs != ESlateBrushDrawType::NoDrawType)
		Style.Hovered = HoveredStyle;
	if (PressedStyle.DrawAs != ESlateBrushDrawType::NoDrawType)
		Style.Pressed = PressedStyle;

	// 색상
	Style.Normal.TintColor = ButtonColor;
	Style.Hovered.TintColor = ButtonColor;
	Style.Pressed.TintColor = ButtonColor;

	ButtonMain->SetStyle(Style);
}

void UCustomButtonWidget::ApplyText()
{
	if (!TextMain) return;

	if (bShowText)
	{
		TextMain->SetText(ButtonText);
		TextMain->SetColorAndOpacity(TextColor);

		FSlateFontInfo FontInfo = TextMain->Font;
		FontInfo.Size = FontSize;
		TextMain->SetFont(FontInfo);

		TextMain->SetJustification(TextJustification);
	}
	else
	{
		TextMain->SetText(FText::GetEmpty());
	}
}
