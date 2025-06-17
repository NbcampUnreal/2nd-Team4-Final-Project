// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

class UImage;
class UTextBlock;
class UCanvasRenderTarget2D;
class UMaterialInterface;
class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    /** 월드 좌표 → 맵 내 UV 좌표 (0~1) 변환 함수 */
    UFUNCTION(BlueprintCallable, Category = "Map")
    FVector2D WorldToMapUV(const FVector& WorldLocation) const;

    /** 플레이어 마커용 → 맵 내 UV 좌표 (0~1) 변환 함수 */
    UFUNCTION(BlueprintCallable, Category = "Map")
    FVector2D WorldToMapScreenPosition(const FVector& WorldLocation) const;

    /** 줌 레벨 변경 */
    UFUNCTION(BlueprintCallable, Category = "Map|Zoom")
    void SetZoom(float NewZoom);

protected:
    /** 위젯 생성 시 초기화 */
    virtual void NativeConstruct() override;

    /** 마우스 바인딩 */
    virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    /** 플레이어 마커 갱신 */
    void UpdatePlayerMarker();

    /** 맵 위치 이동 시 이미지 위치 제한 */
    void SetMapImagePositionClamped(const FVector2D& DesiredPosition);

    /** 일정 주기로 플레이어 위치에 밝은 원을 그림 */
    UFUNCTION()
    void DrawRevealAtPlayerLocation();

protected:
    // -----------------------------
    // UMG 바인딩 컴포넌트들
    // -----------------------------

    /** 월드맵 이미지 (배경) */
    UPROPERTY(meta = (BindWidget))
    UImage* MapImage;

    /** 안개 이미지 (위에 덮여서 어둡게 표시됨) */
    UPROPERTY(meta = (BindWidget))
    UImage* FogImage;

    /** 플레이어 위치 마커 */
    UPROPERTY(meta = (BindWidget))
    UImage* PlayerMarker;

    /** 현재 줌 레벨 텍스트 표시 */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ZoomText;

    // -----------------------------
    // 월드맵 위치 계산 관련 변수
    // -----------------------------

    /** 맵의 월드 기준 시작점 (왼쪽 아래) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    FVector MapOrigin = FVector(-100800.f, -56160.f, 0.f);

    /** 맵이 커버하는 월드 범위 (X, Y 거리) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    FVector2D MapExtent = FVector2D(201600.f, 201600.f);

    /** 맵 이미지 해상도 (픽셀 기준) */
    const FVector2D MapImageSize = FVector2D(1024.f, 1024.f);

    // -----------------------------
    // 줌 관련 설정
    // -----------------------------

    /** 현재 줌 레벨 (1.0 = 기본 배율) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Zoom")
    float ZoomLevel = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Zoom")
    float MinZoom = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Zoom")
    float MaxZoom = 4.0f;

    // -----------------------------
    // 마우스 드래그 관련 변수
    // -----------------------------

    UPROPERTY(EditAnywhere, Category = "Map|Update")
    float MarkerUpdateInterval = 0.02f;

    bool bDraggingMap = false;
    FVector2D DragStartMousePosition;
    FVector2D DragStartMapPosition;

    FTimerHandle MarkerUpdateTimerHandle;

    // -----------------------------
    // Fog of War 관련 요소
    // -----------------------------

    /** 밝힌 영역을 저장할 Render Target */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fog")
    UCanvasRenderTarget2D* FogRenderTarget;

    /** UI에 표시할 Fog 머티리얼 원본 (M_FogOfWar) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fog")
    UMaterialInterface* FogOfWarMaterialBase;

    /** RenderTarget에 원형 밝기를 찍을 머티리얼 원본 (M_DrawCircle) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fog")
    UMaterialInterface* DrawCircleMaterialBase;

    /** UI 표시용 머티리얼 인스턴스 */
    UPROPERTY()
    UMaterialInstanceDynamic* FogOfWarMID;

    /** 밝기 그리기용 머티리얼 인스턴스 */
    UPROPERTY()
    UMaterialInstanceDynamic* DrawCircleMID;

    /** Fog 머티리얼에 넣을 실제 맵 텍스처 (배경) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
    UTexture2D* MapTexture;

    /** 밝히는 함수 주기적 호출 타이머 */
    FTimerHandle FogDrawTimer;
};
