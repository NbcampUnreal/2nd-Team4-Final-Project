// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MapWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "FogManager.h"

void UMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // FogManager 찾기
    FogManager = Cast<AFogManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AFogManager::StaticClass()));
    if (!FogManager || !FogImage || !FogMaterialBase) return;

    // 머티리얼 인스턴스 생성
    FogMaterialInstance = UMaterialInstanceDynamic::Create(FogMaterialBase, this);
    FogMaterialInstance->SetTextureParameterValue("FogTexture", FogManager->GetFogTexture()); // 직접 텍스처 전달

    FogImage->SetBrushFromMaterial(FogMaterialInstance);
    FogImage->SetRenderScale(FVector2D(ZoomLevel, ZoomLevel));

    // 플레이어 마커 위치 주기적 갱신
    GetWorld()->GetTimerManager().SetTimer(
        MarkerUpdateTimerHandle,
        this,
        &UMapWidget::UpdatePlayerMarker,
        0.05f,
        true
    );
}

void UMapWidget::UpdatePlayerMarker()
{
    APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!Player || !PlayerMarker || !FogImage) return;

    FVector WorldPos = Player->GetActorLocation();

    // 월드 위치를 맵 UV로 변환 (GridOrigin/PixelSize 기반)
    FVector2D WorldXY = FVector2D(WorldPos.X, WorldPos.Y);
    FVector2D Relative = WorldXY + FogManager->GetGridOrigin();
    FVector2D UV = Relative / (FogManager->GetTextureSize() * FogManager->GetPixelSize());

    // 맵 이미지에서 위치 계산
    FVector2D MarkerPos = UV * MapImageSize * ZoomLevel;

    if (UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(PlayerMarker->Slot))
    {
        MarkerSlot->SetPosition(MarkerPos);
    }
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
