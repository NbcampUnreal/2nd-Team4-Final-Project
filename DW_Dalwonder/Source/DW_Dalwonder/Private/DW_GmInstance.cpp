// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_GmInstance.h"
//#include "UI/Widget/LoadingWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UDW_GmInstance::UDW_GmInstance()
{
    /*static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/BluePrint/UI/Widget/WBP_LoadingWidget"));
    if (WidgetClass.Succeeded())
    {
        LoadingWidgetClass = WidgetClass.Class;
    }*/
}

void UDW_GmInstance::Init()
{
    Super::Init();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UDW_GmInstance::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UDW_GmInstance::EndLoadingScreen);
}

void UDW_GmInstance::BeginLoadingScreen(const FString& MapName)
{
    UE_LOG(LogTemp, Warning, TEXT("UDW_GmInstance::BeginLoadingScreen : &s"), *MapName);

    UE_LOG(LogTemp, Warning, TEXT("Loading Screen 시작: %s"), *MapName);

    if (!LoadingWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("LoadingWidgetClass is NULL"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("GetWorld() 실패"));
        return;
    }

    UGameViewportClient* Viewport = World->GetGameViewport();
    if (!Viewport)
    {
        UE_LOG(LogTemp, Error, TEXT("Viewport 없음"));
        return;
    }

    UUserWidget* Widget = CreateWidget<UUserWidget>(World, LoadingWidgetClass);
    if (!Widget)
    {
        UE_LOG(LogTemp, Error, TEXT("CreateWidget 실패"));
        return;
    }

    LoadingSlateWidget = Widget->TakeWidget();
    Viewport->AddViewportWidgetContent(LoadingSlateWidget.ToSharedRef(), 100);
    LoadingWidget = Widget;

    UE_LOG(LogTemp, Warning, TEXT("로딩 위젯 Viewport에 추가 완료"));
}

void UDW_GmInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
    UE_LOG(LogTemp, Warning, TEXT("UDW_GmInstance::EndLoadingScreen : &s"), *InLoadedWorld->GetName());

    if (UWorld* World = InLoadedWorld)
    {
        if (UGameViewportClient* Viewport = World->GetGameViewport())
        {
            if (LoadingSlateWidget.IsValid())
            {
                Viewport->RemoveViewportWidgetContent(LoadingSlateWidget.ToSharedRef());
                LoadingSlateWidget.Reset();
            }

            if (LoadingWidget.IsValid())
            {
                LoadingWidget->RemoveFromRoot(); // GC에 맡김
                LoadingWidget = nullptr;
            }
        }
    }
}
