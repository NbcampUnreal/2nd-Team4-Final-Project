#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Sound/SoundBase.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateColor.h"
#include "Styling/SlateTypes.h"
#include "CustomButtonWidget.generated.h"

/** 버튼 클릭 시 외부에서 바인딩 가능한 이벤트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomClicked);

/**
 * UCustomButtonWidget
 * 커스텀 버튼 위젯 - 클릭/호버 사운드 + 텍스트/스타일 설정 가능
 */
UCLASS()
class DW_DALWONDER_API UCustomButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ----------------------------- //
	//          위젯 바인딩
	// ----------------------------- //
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* ButtonMain;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* TextMain;

	// ----------------------------- //
	//         사용자 설정값
	// ----------------------------- //

	// 텍스트 표시 여부
UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button")
bool bShowText = true;

// 버튼 텍스트
UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button")
FText ButtonText = FText::FromString(TEXT("Button"));

// 텍스트 색상
UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button")
FSlateColor TextColor = FLinearColor::White;

// 폰트 크기
UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button")
int32 FontSize = 24;

// 텍스트 정렬
UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button")
TEnumAsByte<ETextJustify::Type> TextJustification = ETextJustify::Center;

// 버튼 색상
UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button")
FSlateColor ButtonColor = FLinearColor::White;

// 사운드
UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button|Sound")
USoundBase* ClickSound;

UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button|Sound")
USoundBase* HoverSound;

// 버튼 스타일
UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button|Style")
FSlateBrush NormalStyle;

UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button|Style")
FSlateBrush HoveredStyle;

UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Custom Button|Style")
FSlateBrush PressedStyle;


	// ----------------------------- //
	//        디스패처 (외부바인딩)
	// ----------------------------- //
	UPROPERTY(BlueprintAssignable, Category = "Custom Button")
	FOnCustomClicked OnCustomClicked;

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void HandleClicked();

	UFUNCTION()
	void HandleHovered();

	/** 버튼 스타일 반영 */
	void ApplyStyle();

	/** 텍스트 설정 반영 */
	void ApplyText();
};
