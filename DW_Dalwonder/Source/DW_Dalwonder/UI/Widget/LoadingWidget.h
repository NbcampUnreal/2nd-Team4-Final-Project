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

    // BP에서 구현: 텍스트 바꾸기
    UFUNCTION(BlueprintImplementableEvent, Category = "Loading")
    void OnTipUpdated(const FString& TipText);

    // 위젯 시작 시 호출
    virtual void NativeConstruct() override;

private:
    void UpdateRandomTip();

    FTimerHandle TipTimerHandle;

    TArray<FString> TipList = {
        TEXT("TIP: 구르기는 무적 프레임이 있어요!"),
        TEXT("TIP: 적의 패턴을 관찰하세요."),
        TEXT("TIP: 강공격은 스태미너를 많이 소모합니다."),
        TEXT("TIP: 스킬트리는 초기화할 수 없어요!"),
        TEXT("TIP: 무기를 강화하면 큰 도움이 됩니다!")
    };

};
