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

    /** �� �̵� �ӵ� (�ʴ� �ȼ�) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher")
    float StarMoveSpeed = 300.f;

    /** ���� ������ ũ�� (�ȼ� ����) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher")
    float SuccessZoneWidth = 100.f;

    /** ���� ������ ��ġ (0 ~ 1) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher")
    float SuccessZoneRatio = 0.5f;

    /** ���� �� �߻��� ����Ʈ �迭 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher")
    TArray<UNiagaraSystem*> SuccessEffects;

    /** �� �̹��� ���� */
    UPROPERTY(meta = (BindWidget))
    class UImage* StarImage;

    /** ���� �����̴� ĵ���� */
    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* StarCanvas;

    /** ���� ���� �̹��� */
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