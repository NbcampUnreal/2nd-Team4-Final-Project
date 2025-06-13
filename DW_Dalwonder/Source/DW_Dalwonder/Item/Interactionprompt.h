// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactionprompt.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UInteractionprompt : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "World Item Data")
	class AWorldItemActor* OwningWorldItemActor;

	UFUNCTION(BlueprintImplementableEvent, Category = "MyEvents")
	void InitializeData();
};
