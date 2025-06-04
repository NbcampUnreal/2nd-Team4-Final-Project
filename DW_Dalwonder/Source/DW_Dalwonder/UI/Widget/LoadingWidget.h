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
	// 퍼센트 값 업데이트 (0~1)
	UFUNCTION(BlueprintImplementableEvent, Category = "Loading")
	void OnProgressUpdated(float Progress);

//public:
//    UFUNCTION(BlueprintCallable, Category = "Loading")
//    void UpdateProgress(float InPercent);
//
//protected:
//    // 블루프린트에서 구현할 이벤트
//    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
//    void OnProgressUpdated(float Progress);
//    virtual void OnProgressUpdated_Implementation(float Progress);

};
