// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SavedGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API USavedGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveData1;

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveData2;

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveData3;

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveData4;
};
