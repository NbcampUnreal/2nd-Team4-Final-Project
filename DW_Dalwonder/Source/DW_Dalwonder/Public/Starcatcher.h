// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Input/Reply.h"
#include "Blueprint/UserWidget.h"
#include "Starcatcher.generated.h"

UCLASS()
class DW_DALWONDER_API UStarcatcher : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry,const FKeyEvent& InKeyEvent) override;

    /** 별 이동 속도 (초당 픽셀) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher")
    float StarMoveSpeed = 300.f;

    /** 성공 영역의 크기 (픽셀 단위) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher")
    float SuccessZoneWidth = 100.f;

    /** 성공 영역의 위치 (0 ~ 1) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher")
    float SuccessZoneRatio = 0.5f;

    /** 성공 시 발생할 이펙트 배열 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher")
    TArray<UNiagaraSystem*> SuccessEffects;

    /** 별 이미지 위젯 */
    UPROPERTY(meta = (BindWidget))
    class UImage* StarImage;

    /** 별이 움직이는 캔버스 */
    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* StarCanvas;

    /** 성공 구역 이미지 */
    UPROPERTY(meta = (BindWidgetOptional))
    class UImage* SuccessZoneImage;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStarCatcherFinished, int32, SuccessBonus);
    UPROPERTY(BlueprintAssignable, Category = "Starcatcher")
    FOnStarCatcherFinished OnStarCatcherFinished;

private:
    float CurrentStarX = 0.f;
    bool bGoingRight = true;

    int32 SuccessCount = 0;
    int32 CurrentTry = 0;
    const int32 MaxTry = 3;
    bool bIsGameFinished = false;

    void HandleInput();
    void FinishGame();
};