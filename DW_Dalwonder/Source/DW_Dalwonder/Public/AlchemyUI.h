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

    /** 스타캐처 위젯 클래스 (디자이너에서 설정) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UStarcatcher> StarcatcherClass;

    /** 시작 버튼 */
    UPROPERTY(meta = (BindWidget))
    UButton* StartButton;

    /** 확률 텍스트 표시 */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Percent;
};