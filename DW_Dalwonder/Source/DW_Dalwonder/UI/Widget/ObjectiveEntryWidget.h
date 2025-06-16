// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Component/Struct/QuestObjective.h"
#include "ObjectiveEntryWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UObjectiveEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void InitWithObjective(const FQuestObjective& ObjectiveData);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ObjectiveText;
};
