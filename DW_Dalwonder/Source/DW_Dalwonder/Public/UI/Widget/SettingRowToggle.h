// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingRowToggle.generated.h"

class UCheckBox;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggleChanged, bool, bNewState);

UCLASS()
class DW_DALWONDER_API USettingRowToggle : public UUserWidget
{
	GENERATED_BODY()

public:
	// BindWidget
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextLabel;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBox;

	// 상태 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Toggle")
	FText LabelText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Toggle")
	bool bIsOn = true;

	// 외부 바인딩용 디스패처
	UPROPERTY(BlueprintAssignable, Category = "Toggle")
	FOnToggleChanged OnToggleChanged;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void HandleToggleChanged(bool bNewChecked);
};
