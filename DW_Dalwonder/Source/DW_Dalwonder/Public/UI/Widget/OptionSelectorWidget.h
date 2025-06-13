// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionSelectorWidget.generated.h"
class UHorizontalBox;
class UTextBlock;
class UComboBoxString;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionSelected, int32, SelectedIndex);

UCLASS()
class DW_DALWONDER_API UOptionSelectorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 바인딩된 위젯
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextLabel;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBoxOptions;

	// 설정 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Option")
	FText LabelText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Option")
	TArray<FText> OptionList;

	// 선택 이벤트 디스패처
	UPROPERTY(BlueprintAssignable, Category = "Option")
	FOnOptionSelected OnOptionSelected;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	// 선택 이벤트 핸들러
	UFUNCTION()
	void OnOptionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	// 선택 상태 추적 (필요 시)
	int32 CurrentSelectedIndex = INDEX_NONE;
};