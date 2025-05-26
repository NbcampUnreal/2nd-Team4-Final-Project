// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DW_GmInstance.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDW_GmInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

    UDW_GmInstance();

    virtual void Init() override;

    /** OpenLevel 전에 로딩 위젯을 띄움 */
    UFUNCTION()
    virtual void BeginLoadingScreen(const FString& MapName);

    /** OpenLevel 후 로딩 위젯을 제거함 */
    UFUNCTION()
    virtual void EndLoadingScreen(UWorld* InLoadedWorld);

private:
    /** GameViewport에 붙는 Slate 위젯 (UMG → Slate로 변환된 위젯) */
    TSharedPtr<SWidget> LoadingSlateWidget;

    /** 실제 UMG 로딩 위젯 클래스 */
    UPROPERTY(EditDefaultsOnly, Category = "Loading")
    TSubclassOf<UUserWidget> LoadingWidgetClass;

    /** 생성된 로딩 위젯 인스턴스 (UMG) */
    TWeakObjectPtr<UUserWidget> LoadingWidget;

};
