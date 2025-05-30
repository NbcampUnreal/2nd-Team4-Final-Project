// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


void ALoadBase::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (LoadingWidgetClass)
    {
        UUserWidget* Widget = CreateWidget<UUserWidget>(PC, LoadingWidgetClass);
        if (Widget)
        {
            Widget->AddToViewport();
        }
    }

    // 2초 후 GameMap으로 이동
    FTimerHandle Handle;
    GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([]()
        {
            UGameplayStatics::OpenLevel(GWorld, "Start_Testmap_WP"); // 오픈월드 맵 이름
        }), 2.0f, false);
}
