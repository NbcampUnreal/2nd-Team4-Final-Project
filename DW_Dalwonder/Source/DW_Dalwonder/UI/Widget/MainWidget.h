// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

class UCustomButtonWidget;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	// 버튼 클릭 이벤트 함수
	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnContinueGameClicked();

	UFUNCTION()
	void OnOptionClicked();

	UFUNCTION()
	void OnExitClicked();

public:

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* NewGameButton;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* ContinueGameButton;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* OptionButton;

	UPROPERTY(meta = (BindWidget))
	class UCustomButtonWidget* ExitButton;

	// 연결 위젯들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> SavedGameWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UUserWidget> OptionMenuWidgetClass;
};
