// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlchemyUI.generated.h"

class UButton;
class UTextBlock;
class UStarcatcher;

UCLASS()
class DW_DALWONDER_API UAlchemyUI : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    UFUNCTION()
    void OnStartButtonClicked();

    UFUNCTION()
    void OnStarcatcherFinished(int32 SuccessCount);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UStarcatcher> StarcatcherClass;

    UPROPERTY(meta = (BindWidget))
    UButton* StartButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Percent;
};