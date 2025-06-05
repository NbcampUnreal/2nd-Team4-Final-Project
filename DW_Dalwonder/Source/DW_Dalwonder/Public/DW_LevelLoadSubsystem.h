// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/LevelStreamingDynamic.h"
#include "DW_LevelLoadSubsystem.generated.h"

class ULoadingWidget;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDW_LevelLoadSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    // 맵 스트리밍 및 로딩 위젯 시작
    UFUNCTION(BlueprintCallable)
    void StreamLevelAsync(FName LevelName);

    // 로딩 위젯 클래스 설정 (GameInstance에서 받아옴 호출)
    void SetLoadingWidgetClass(TSubclassOf<ULoadingWidget> InClass);

    // 캐릭터 스폰 조정
    UFUNCTION(BlueprintCallable)
    void SpawnPlayerCharacterAtPlayerStart();

    // PlayerStart로딩될때까지 기다리기
    UFUNCTION(BlueprintCallable)
    void WaitForPlayerStartThenSpawn();

protected:
    // 로딩 상태 업데이트 (Tick에서 호출됨)
    void TickStreamingProgress();

private:
    // 스트리밍 레벨 핸들
    UPROPERTY()
    ULevelStreamingDynamic* StreamingLevel = nullptr;

    // 진행률 추적용 변수
    float CurrentProgress = 0.f;

    // 로딩 진행 타이머 핸들
    FTimerHandle TickTimerHandle;

    // 로딩 UI
    UPROPERTY()
    ULoadingWidget* LoadingWidget = nullptr;

    // 로딩 위젯 클래스 (BP 기반 GameInstance에서 받아옴)
    UPROPERTY(EditAnywhere)
    TSubclassOf<ULoadingWidget> LoadingWidgetClass;

};
