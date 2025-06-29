// Fill out your copyright notice in the Description page of Project Settings.


#include "FogManager.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "EngineUtils.h"
#include "Landscape.h"
#include "Character/DW_CharacterBase.h"

// Sets default values
AFogManager::AFogManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AFogManager::BeginPlay()
{
    Super::BeginPlay();

    InitFog();

    // 일정 주기로 TryRevealFog 실행(위치 확인하고 안개밝히기)
    GetWorld()->GetTimerManager().SetTimer(
        FogCheckTimerHandle,
        this,
        &AFogManager::TryRevealFog,
        RevealInterval,
        true
    );
}

void AFogManager::InitFog()
{
    // === [1] 모든 Landscape Bounds 계산 ===
    FBox CombinedLandscapeBounds(EForceInit::ForceInit);
    for (TActorIterator<ALandscape> It(GetWorld()); It; ++It)
    {
        ALandscape* Landscape = *It;
        if (Landscape)
        {
            CombinedLandscapeBounds += Landscape->GetComponentsBoundingBox(true);
        }
    }

    // === [2] Landscape가 없을 경우 fallback ===
    if (!CombinedLandscapeBounds.IsValid)
    {
        CombinedLandscapeBounds = FBox(FVector(-50000, -50000, 0), FVector(50000, 50000, 0)); // 예시: 1km x 1km
    }

    // === [3] GridOrigin, TextureSize 자동 설정 ===
    FVector BoundsMin = CombinedLandscapeBounds.Min;
    FVector BoundsSize = CombinedLandscapeBounds.GetSize();

    GridOrigin = FVector2D(BoundsMin.X, BoundsMin.Y);
    //GridOrigin = FVector2D(-BoundsMin.X, -BoundsMin.Y); // 월드좌표계 → 상대좌표계
    TextureSize = FMath::CeilToInt(FMath::Max(BoundsSize.X, BoundsSize.Y) / PixelSize);

    // === [4] 텍스처 생성 ===
    FogTexture = UTexture2D::CreateTransient(TextureSize, TextureSize, PF_B8G8R8A8);
    FogTexture->MipGenSettings = TMGS_NoMipmaps;
    FogTexture->CompressionSettings = TC_VectorDisplacementmap;
    FogTexture->SRGB = false;
    FogTexture->AddToRoot();
    FogTexture->UpdateResource();

    // === [5] 픽셀 배열 초기화 ===
    Pixels.SetNumZeroed(TextureSize * TextureSize);
    for (FColor& Pixel : Pixels)
    {
        Pixel = FColor::Black; // 처음엔 모두 어둡게
    }

    UpdateTexture();
}

void AFogManager::TryRevealFog()
{
    ADW_CharacterBase* PlayerChar = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerChar) return;

    FVector WorldLocation = PlayerChar->GetActorLocation();

    // GridOrigin 기준 상대 위치
    FVector2D RelativeLocation = FVector2D(WorldLocation.X, WorldLocation.Y) - GridOrigin;

    int32 GridX = FMath::FloorToInt(RelativeLocation.X / PixelSize);
    int32 GridY = FMath::FloorToInt(RelativeLocation.Y / PixelSize);

    FIntPoint CurrentGrid(GridX, GridY);

    if (CurrentGrid != LastRevealedGrid)
    {
        LastRevealedGrid = CurrentGrid;

        UpdateFogAtPlayerLocation(WorldLocation);
        RevealedGrids.Add(CurrentGrid);

        UE_LOG(LogTemp, Log, TEXT("Revealed Grid: (%d, %d)"), GridX, GridY);
    }

    //ADW_CharacterBase* PlayerChar = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    //if (!PlayerChar) return;

    //FVector WorldLocation = PlayerChar->GetActorLocation();
    //FVector2D RelativeLocation = FVector2D(WorldLocation.X, WorldLocation.Y) + GridOrigin;

    //int32 GridX = FMath::FloorToInt(RelativeLocation.X / PixelSize);
    //int32 GridY = FMath::FloorToInt(RelativeLocation.Y / PixelSize);

    //FIntPoint CurrentGrid(GridX, GridY);

    //// 새로운 셀에 도달한 경우에만 밝히기
    //if (CurrentGrid != LastRevealedGrid)
    //{
    //    LastRevealedGrid = CurrentGrid;

    //    UpdateFogAtPlayerLocation(WorldLocation);
    //    RevealedGrids.Add(CurrentGrid);

    //    UE_LOG(LogTemp, Log, TEXT("Revealed Grid: (%d, %d)"), GridX, GridY);
    //}
}

void AFogManager::UpdateFogAtPlayerLocation(const FVector& WorldLocation)
{
    // GridOrigin 기준 상대 위치
    FVector2D RelativeLocation = FVector2D(WorldLocation.X, WorldLocation.Y) - GridOrigin;

    int32 CenterX = FMath::FloorToInt(RelativeLocation.X / PixelSize);
    int32 CenterY = FMath::FloorToInt(RelativeLocation.Y / PixelSize);
    int32 RadiusInPixels = FMath::CeilToInt(RevealRadius / PixelSize);

    for (int32 Y = -RadiusInPixels; Y <= RadiusInPixels; ++Y)
    {
        for (int32 X = -RadiusInPixels; X <= RadiusInPixels; ++X)
        {
            int32 GridX = CenterX + X;
            int32 GridY = CenterY + Y;

            float DistSqr = X * X + Y * Y;
            if (DistSqr > RadiusInPixels * RadiusInPixels)
                continue;

            int32 TextureX = GridY;
            int32 TextureY = TextureSize - GridX - 1;

            if (TextureX < 0 || TextureY < 0 || TextureX >= TextureSize || TextureY >= TextureSize)
                continue;

            int32 Index = TextureY * TextureSize + TextureX;
            Pixels[Index] = FColor::White;
        }
    }

    UpdateTexture();

    //FVector2D RelativeLocation = FVector2D(WorldLocation.X, WorldLocation.Y) + GridOrigin;

    //int32 CenterX = FMath::FloorToInt(RelativeLocation.X / PixelSize);
    //int32 CenterY = FMath::FloorToInt(RelativeLocation.Y / PixelSize);
    //int32 RadiusInPixels = FMath::CeilToInt(RevealRadius / PixelSize);

    //for (int32 Y = -RadiusInPixels; Y <= RadiusInPixels; ++Y)
    //{
    //    for (int32 X = -RadiusInPixels; X <= RadiusInPixels; ++X)
    //    {
    //        int32 GridX = CenterX + X;
    //        int32 GridY = CenterY + Y;

    //        float DistSqr = X * X + Y * Y;
    //        if (DistSqr > RadiusInPixels * RadiusInPixels)
    //            continue;

    //        // 좌표 보정
    //        int32 TextureX = GridY;
    //        int32 TextureY = TextureSize - GridX - 1;

    //        if (TextureX < 0 || TextureY < 0 || TextureX >= TextureSize || TextureY >= TextureSize)
    //            continue;

    //        int32 Index = TextureY * TextureSize + TextureX;
    //        Pixels[Index] = FColor::White;
    //    }
    //}

    //UpdateTexture();
}

void AFogManager::UpdateTexture()
{
    if (!FogTexture || Pixels.Num() == 0)
        return;

    FTexture2DMipMap& Mip = FogTexture->GetPlatformData()->Mips[0];
    void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(TextureData, Pixels.GetData(), Pixels.Num() * sizeof(FColor));
    Mip.BulkData.Unlock();

    FogTexture->UpdateResource();
}

