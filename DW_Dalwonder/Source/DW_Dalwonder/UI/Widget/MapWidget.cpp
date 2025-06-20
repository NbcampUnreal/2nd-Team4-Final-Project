// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MapWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

void UMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // -----------------------------
    // 1️. RenderTarget 생성 (안개 상태 저장용)
    // -----------------------------
    FogRenderTarget = UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(
        this,
        UCanvasRenderTarget2D::StaticClass(),
        1024, 1024
    );
    FogRenderTarget->ClearColor = FLinearColor::Black; // 전체 어둡게 시작
    FogRenderTarget->UpdateResource();

    // -----------------------------
    // 2️. 밝히기용 머티리얼 인스턴스 생성 (DrawCircle)
    // -----------------------------
    if (DrawCircleMaterialBase)
    {
        DrawCircleMID = UMaterialInstanceDynamic::Create(DrawCircleMaterialBase, this);
    }

    // -----------------------------
    // 3️. Fog 가시화 머티리얼 인스턴스 생성 (FogOfWar)
    // -----------------------------
    if (FogOfWarMaterialBase)
    {
        FogOfWarMID = UMaterialInstanceDynamic::Create(FogOfWarMaterialBase, this);
        FogOfWarMID->SetTextureParameterValue(TEXT("FogMask"), FogRenderTarget);

        if (MapTexture)
        {
            FogOfWarMID->SetTextureParameterValue(TEXT("MapTexture"), MapTexture);
        }

        if (FogImage)
        {
            FogImage->SetBrushFromMaterial(FogOfWarMID);
        }
    }

    // -----------------------------
    // 4️. 플레이어 위치 밝히기 함수 타이머 시작
    // -----------------------------
    GetWorld()->GetTimerManager().SetTimer(
        FogDrawTimer,
        this,
        &UMapWidget::DrawRevealAtPlayerLocation,
        0.2f, // 5fps 정도
        true
    );

    // -----------------------------
    // 5️. 플레이어 마커 초기화
    // -----------------------------
    if (PlayerMarker)
    {
        if (UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(PlayerMarker->Slot))
        {
            MarkerSlot->SetAlignment(FVector2D(0.5f, 0.5f));
        }

        UpdatePlayerMarker();
        SetZoom(ZoomLevel);
    }

    SetVisibility(ESlateVisibility::Visible);
    SetIsFocusable(true);
}

FReply UMapWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    float ScrollDelta = InMouseEvent.GetWheelDelta(); // +1 or -1
    float NewZoom = ZoomLevel + ScrollDelta * 0.1f;   // 줌 스피드 조절

    SetZoom(NewZoom);

    return FReply::Handled();
}

FReply UMapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bDraggingMap = true;
        DragStartMousePosition = InMouseEvent.GetScreenSpacePosition();

        if (FogImage)
        {
            if (UCanvasPanelSlot* CSlot = Cast<UCanvasPanelSlot>(FogImage->Slot))
            {
                DragStartMapPosition = CSlot->GetPosition();
            }
        }

        // 마커 업데이트용 타이머 시작
        GetWorld()->GetTimerManager().SetTimer(
            MarkerUpdateTimerHandle,
            this,
            &UMapWidget::UpdatePlayerMarker,
            MarkerUpdateInterval,
            true // looping
        );

        return FReply::Handled();
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UMapWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bDraggingMap && FogImage)
    {
        FVector2D CurrentMousePos = InMouseEvent.GetScreenSpacePosition();
        FVector2D MouseDelta = CurrentMousePos - DragStartMousePosition;

        if (UCanvasPanelSlot* CSlot = Cast<UCanvasPanelSlot>(FogImage->Slot))
        {
            FVector2D NewPos = DragStartMapPosition + MouseDelta;
            SetMapImagePositionClamped(NewPos);

            CSlot->SetPosition(NewPos);
        }

        return FReply::Handled();
    }

    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UMapWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bDraggingMap)
    {
        bDraggingMap = false;
        GetWorld()->GetTimerManager().ClearTimer(MarkerUpdateTimerHandle);
        return FReply::Handled().ReleaseMouseCapture();
    }

    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

// 플레이어 위치를 UV 기준으로 변환하고 해당 위치에 밝은 원을 찍는다
void UMapWidget::DrawRevealAtPlayerLocation()
{
    if (!DrawCircleMID || !FogRenderTarget) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!PlayerPawn) return;

    FVector2D UV = WorldToMapUV(PlayerPawn->GetActorLocation());

    DrawCircleMID->SetVectorParameterValue(TEXT("CirclePos"), FLinearColor(UV.X, UV.Y, 0, 0));
    DrawCircleMID->SetScalarParameterValue(TEXT("Radius"), 0.05f); // 밝기 반경

    // RenderTarget에 원형 밝기 머티리얼 그리기
    UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, FogRenderTarget, DrawCircleMID);
}

// 월드 좌표를 맵 UV 좌표 (0~1)로 변환
FVector2D UMapWidget::WorldToMapUV(const FVector& WorldLocation) const
{
    FVector2D WorldXY(WorldLocation.X, WorldLocation.Y);
    FVector2D OriginXY(MapOrigin.X, MapOrigin.Y);
    FVector2D Delta = WorldXY - OriginXY;
    return Delta / MapExtent; // 0~1 범위
}

FVector2D UMapWidget::WorldToMapScreenPosition(const FVector& WorldLocation) const
{
    FVector2D WorldXY(WorldLocation.X, WorldLocation.Y);
    FVector2D OriginXY(MapOrigin.X, MapOrigin.Y);
    FVector2D Delta = WorldXY - OriginXY;
    FVector2D Normalized = Delta / MapExtent;
    FVector2D ScaledSize = MapImageSize * ZoomLevel;

    return Normalized * ScaledSize;
}

// 맵 이미지 줌 처리
void UMapWidget::SetZoom(float NewZoom)
{
    ZoomLevel = FMath::Clamp(NewZoom, MinZoom, MaxZoom);
    
    if (FogImage)
    {
        FogImage->SetRenderScale(FVector2D(ZoomLevel, ZoomLevel));

        if (UCanvasPanelSlot* MapSlot = Cast<UCanvasPanelSlot>(FogImage->Slot))
        {
            SetMapImagePositionClamped(MapSlot->GetPosition());
        }
    }

    if (ZoomText)
    {
        int32 ZoomPercent = FMath::RoundToInt(ZoomLevel * 100.f);
        ZoomText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), ZoomPercent)));
    }

    UpdatePlayerMarker();
}

// 마커 위치 화면에 표시
void UMapWidget::UpdatePlayerMarker()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!PlayerPawn || !PlayerMarker || !MapImage) return;

    FVector WorldLocation = PlayerPawn->GetActorLocation();
    FVector2D MapCoord = WorldToMapScreenPosition(WorldLocation); // 0~1024 기준

    // 오프셋: 이미지 중앙 기준 정렬 → 좌상단 기준으로 보정
    FVector2D MapOffset = MapImageSize * ZoomLevel * -0.5f;
    FVector2D FinalCoord = MapCoord + MapOffset;

    // MapImage의 현재 위치도 보정에 더해줘야 함
    if (UCanvasPanelSlot* MapSlot = Cast<UCanvasPanelSlot>(FogImage->Slot))
    {
        FinalCoord += MapSlot->GetPosition();
    }

    // 클램핑을 FinalCoord에 적용
    FinalCoord.X = FMath::Clamp(FinalCoord.X, MapOffset.X, MapOffset.X + MapImageSize.X * ZoomLevel);
    FinalCoord.Y = FMath::Clamp(FinalCoord.Y, MapOffset.Y, MapOffset.Y + MapImageSize.Y * ZoomLevel);

    if (UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(PlayerMarker->Slot))
    {
        MarkerSlot->SetPosition(FinalCoord); // 보정된 좌표 사용
    }
}

// 맵 이동 시 이미지 위치 제한
void UMapWidget::SetMapImagePositionClamped(const FVector2D& DesiredPosition)
{
    if (!FogImage) return;

    if (UCanvasPanelSlot* MapSlot = Cast<UCanvasPanelSlot>(FogImage->Slot))
    {
        FVector2D MapSize = MapImageSize * ZoomLevel;
        FVector2D ViewportSize = FVector2D(1024.f, 1024.f); // 필요 시 변수화 가능

        FVector2D MinPos = ViewportSize - MapSize;
        FVector2D MaxPos = FVector2D(0.f, 0.f);

        FVector2D Clamped = FVector2D(
            FMath::Clamp(DesiredPosition.X, MinPos.X, MaxPos.X),
            FMath::Clamp(DesiredPosition.Y, MinPos.Y, MaxPos.Y)
        );

        MapSlot->SetPosition(Clamped);
    }
}
