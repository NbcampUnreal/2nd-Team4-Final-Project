// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "FogManager.generated.h"

/**
 * AFogManager는 월드 내 안개(Fog of War)를 관리하는 액터입니다.
 * - 일정 시간 간격으로 플레이어 위치를 검사하여
 * - 새로운 그리드 셀에 도달하면 해당 영역을 밝힙니다.
 * - 결과는 Texture2D로 관리되며 UI 등에서 시각화 가능
 */
UCLASS()
class DW_DALWONDER_API AFogManager : public AActor
{
	GENERATED_BODY()
	
public:
    AFogManager();

    virtual void BeginPlay() override;

    // 초기화: 텍스처 및 픽셀 데이터 설정
    UFUNCTION(BlueprintCallable)
    void InitFog();

    // 월드 위치 기준으로 안개 밝히기
    void UpdateFogAtPlayerLocation(const FVector& WorldLocation);

    // 맵 이동후 지도 복원(Bitmask로 복원)
    void ApplyRevealedFogFromGameInstance();

    /** FogTexture 반환 */
    UTexture2D* GetFogTexture() const { return FogTexture; }

    /** GridOrigin 반환 */
    FVector2D GetGridOrigin() const { return GridOrigin; }

    /** 픽셀당 월드 거리 반환 */
    float GetPixelSize() const { return PixelSize; }

    /** 텍스처 해상도 반환 */
    int32 GetTextureSize() const { return TextureSize; }

protected:
    // 픽셀 데이터 → 텍스처에 반영
    void UpdateTexture();

    // 타이머로 주기적으로 위치 검사 후 밝힘
    void TryRevealFog();

protected:
    // 안개 텍스처
    UPROPERTY()
    UTexture2D* FogTexture;

    // 픽셀 배열 (흑: 안보임, 백: 밝혀짐)
    TArray<FColor> Pixels;

    // 플레이어 위치 체크 타이머
    FTimerHandle FogCheckTimerHandle;

    // 마지막으로 밝힌 그리드 셀
    FIntPoint LastRevealedGrid;

    // 안개 범위 설정값들
    UPROPERTY(EditAnywhere, Category = "Fog")
    int32 TextureSize = 2048;

    UPROPERTY(EditAnywhere, Category = "Fog")
    float PixelSize = 2000.0f;

    UPROPERTY(EditAnywhere, Category = "Fog")
    float RevealRadius = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Fog")
    float RevealInterval = 0.1f;

    // 그리드 원점 (월드 좌표계)
    UPROPERTY(EditAnywhere, Category = "Fog")
    FVector2D GridOrigin = FVector2D::ZeroVector;
};
