// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    virtual void NativeConstruct() override;

    // 퍼센트를 설정하는 함수
    UFUNCTION(BlueprintCallable)
    void SetLoadingProgress(float InPercent);

private:
    // 위젯 바인딩용 변수
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* LoadingProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LoadingText;
};
