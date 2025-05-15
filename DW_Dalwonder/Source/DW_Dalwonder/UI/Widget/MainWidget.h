// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UButton* NewGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

};
