// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

class UImage;
class UTextBlock;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class AFogManager;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 시작 및 종료
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

	/** 마우스 휠 이벤트 처리 */
    virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    /** 플레이어 위치 갱신 */
    UFUNCTION()
    void UpdatePlayerMarker();

    /** 맵 확대/축소 */
    UFUNCTION(BlueprintCallable)
    void SetZoom(float NewZoom);

protected:

	/** 맵 이미지 표시용 이미지 */
    UPROPERTY(meta = (BindWidget))
    UImage* MapImage;

    /** Fog 표시용 이미지 */
    UPROPERTY(meta = (BindWidget))
    UImage* FogImage;

    /** 플레이어 마커 */
    UPROPERTY(meta = (BindWidget))
    UImage* PlayerMarker;

    /** 줌 텍스트 표시 */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ZoomText;

    /** Fog 머티리얼 원본 (M_FogOfWar) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fog")
    UMaterialInterface* FogMaterialBase;

    /** Fog 머티리얼 인스턴스 */
    UPROPERTY()
    UMaterialInstanceDynamic* FogMaterialInstance;

    /** FogManager 액터 참조 */
    UPROPERTY()
    AFogManager* FogManager;

    /** 플레이어 마커 갱신 타이머 */
    FTimerHandle MarkerUpdateTimerHandle;

    /** 줌 배율 */
    UPROPERTY(EditAnywhere, Category = "Map")
    float ZoomLevel = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Map")
    float MinZoom = 0.5f;

    UPROPERTY(EditAnywhere, Category = "Map")
    float MaxZoom = 2.0f;

    /** 맵 사이즈 (정사각형, 예: 2048x2048) */
    UPROPERTY(EditAnywhere, Category = "Map")
    FVector2D MapImageSize = FVector2D(2048.f, 2048.f);

    FVector2D CurrentOffset = FVector2D::ZeroVector;
    bool bDragging = false;
    FVector2D DragStartPosition;
};
