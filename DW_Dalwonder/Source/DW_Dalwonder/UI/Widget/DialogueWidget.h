// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    /** 텍스트 세팅 함수 */
    UFUNCTION(BlueprintCallable)
    void SetDialogueText(const FText& InText);

    UFUNCTION(BlueprintCallable)
    void SetSpeakerName(const FText& InName);

protected:
    /** 바인딩된 텍스트 블록 */
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DialogueText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SpeakerNameText;
};
