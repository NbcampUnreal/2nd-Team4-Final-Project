// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MapWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Character/DW_CharacterBase.h"
#include "FogManager.h"

void UMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 초기화
    ZoomLevel = 1.0f;
    CurrentOffset = FVector2D::ZeroVector;

    if (MapImage)
    {
        MapImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
    }

    if (FogImage)
    {
        FogImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
    }

    // FogManager 찾기
    FogManager = Cast<AFogManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AFogManager::StaticClass()));
    if (!FogManager || !FogImage || !FogMaterialBase) return;

    // 머티리얼 인스턴스 생성
    FogMaterialInstance = UMaterialInstanceDynamic::Create(FogMaterialBase, this);
    FogMaterialInstance->SetTextureParameterValue("FogTexture", FogManager->GetFogTexture()); // 직접 텍스처 전달

    FogImage->SetBrushFromMaterial(FogMaterialInstance);
    FogImage->SetRenderScale(FVector2D(ZoomLevel, ZoomLevel));

    SetZoom(ZoomLevel);

    // 플레이어 마커 위치 주기적 갱신
    GetWorld()->GetTimerManager().SetTimer(
        MarkerUpdateTimerHandle,
        this,
        &UMapWidget::UpdatePlayerMarker,
        0.05f,
        true
    );
}

void UMapWidget::NativeDestruct()
{
    Super::NativeDestruct();

    // 타이머 정리
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(MarkerUpdateTimerHandle);
    }
}

void UMapWidget::UpdatePlayerMarker()
{
    if (!FogManager || !PlayerMarker || !MapImage) return;

    ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (!Player) return;

    FVector2D WorldXY(Player->GetActorLocation().X, Player->GetActorLocation().Y);
    FVector2D Relative = WorldXY - FogManager->GetGridOrigin();

    float TextureWorldSize = FogManager->GetTextureSize() * FogManager->GetPixelSize();

    // UV 계산
    float U = Relative.Y / TextureWorldSize;
    float V = 1.0f - (Relative.X / TextureWorldSize);
    FVector2D UV = FVector2D(U, V).ClampAxes(0.f, 1.f);

    // 중심 기준 줌 보정
    FVector2D LocalMapSize = MapImageSize * ZoomLevel;
    FVector2D MarkerPos = UV * LocalMapSize;

    // 중앙 기준 확대 보정: 전체 사이즈 변화의 절반만큼 밀림
    FVector2D ZoomOffset = (MapImageSize * (ZoomLevel - 1.0f)) * -0.5f;

    // 최종 위치
    MarkerPos += CurrentOffset + ZoomOffset;

    PlayerMarker->SetRenderTransform(FWidgetTransform{
        MarkerPos,
        FVector2D(1.f, 1.f),
        FVector2D(0.5f, 0.5f),
        0.f
        });

    PlayerMarker->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
}

void UMapWidget::SetZoom(float NewZoom)
{
    ZoomLevel = FMath::Clamp(NewZoom, MinZoom, MaxZoom);

    if (FogImage)
    {
        FogImage->SetRenderScale(FVector2D(ZoomLevel, ZoomLevel));
    }

    if (ZoomText)
    {
        int32 Percent = FMath::RoundToInt(ZoomLevel * 100.f);
        ZoomText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), Percent)));
    }
}

FReply UMapWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    float ScrollDelta = InMouseEvent.GetWheelDelta();
    ZoomLevel = FMath::Clamp(ZoomLevel + ScrollDelta * 0.1f, 0.5f, 3.0f);

    FWidgetTransform NewTransform;
    NewTransform.Translation = CurrentOffset;
    NewTransform.Scale = FVector2D(ZoomLevel, ZoomLevel);

    if (MapImage) MapImage->SetRenderTransform(NewTransform);
    if (FogImage) FogImage->SetRenderTransform(NewTransform);
    SetZoom(ZoomLevel);

    return FReply::Handled();
}

FReply UMapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bDragging = true;
        DragStartPosition = InMouseEvent.GetScreenSpacePosition();
        return FReply::Handled();
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UMapWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bDragging && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bDragging = false;
        return FReply::Handled().ReleaseMouseCapture();
    }
    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UMapWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bDragging)
    {
        FVector2D Delta = InMouseEvent.GetScreenSpacePosition() - DragStartPosition;
        DragStartPosition = InMouseEvent.GetScreenSpacePosition();
        CurrentOffset += Delta;

        FWidgetTransform NewTransform;
        NewTransform.Translation = CurrentOffset;
        NewTransform.Scale = FVector2D(ZoomLevel, ZoomLevel);

        if (MapImage) MapImage->SetRenderTransform(NewTransform);
        if (FogImage) FogImage->SetRenderTransform(NewTransform);

        return FReply::Handled();
    }
    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}
