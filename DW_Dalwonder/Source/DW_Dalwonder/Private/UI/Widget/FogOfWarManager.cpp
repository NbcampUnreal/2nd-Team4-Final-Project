#include "UI/Widget/FogOfWarManager.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AFogOfWarManager::AFogOfWarManager()
{
    PrimaryActorTick.bCanEverTick = true;
    RevealRadius = 5.0f;
    TextureSize = 512;
    PixelSize = 100.0f;

    //Origin 값은 GameMode BeginPlay에서 Player의 위치를 참조하여 정해짐
    GridOrigin = FVector2D(0, 0);
}

void AFogOfWarManager::BeginPlay()
{
    Super::BeginPlay();
}

void AFogOfWarManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    APawn* PlayerPawn = PC->GetPawn();
    if (!PlayerPawn) return;
    
    UpdateFogAtPlayerLocation(PlayerPawn->GetActorLocation());
}

void AFogOfWarManager::UpdateFogAtPlayerLocation(const FVector& WorldLocation)
{
    const FVector2D RelativeLocation = FVector2D(WorldLocation.X, WorldLocation.Y) + GridOrigin;
    const int32 CenterX = FMath::FloorToInt(RelativeLocation.X / PixelSize);
    const int32 CenterY = FMath::FloorToInt(RelativeLocation.Y / PixelSize);
    const int32 RadiusInPixels = FMath::CeilToInt(RevealRadius / PixelSize);

    for (int32 Y = -RadiusInPixels; Y <= RadiusInPixels; ++Y)
    {
        for (int32 X = -RadiusInPixels; X <= RadiusInPixels; ++X)
        {
            const int32 PX = CenterX + X;
            const int32 PY = CenterY + Y;

            if (PX < 0 || PY < 0 || PX >= TextureSize || PY >= TextureSize) continue;

            const float DistSqr = X * X + Y * Y;
            if (DistSqr > RadiusInPixels * RadiusInPixels) continue;

            const int32 Index = PY * TextureSize + PX;
            Pixels[Index] = FColor::White;
        }
    }
    UpdateTexture();
}

void AFogOfWarManager::UpdateTexture()
{
    if (!FogTexture || Pixels.Num() == 0) return;

    FTexture2DMipMap& Mip = FogTexture->GetPlatformData()->Mips[0];
    void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(TextureData, Pixels.GetData(), Pixels.Num() * sizeof(FColor));
    Mip.BulkData.Unlock();
    FogTexture->UpdateResource();
}

UTexture2D* AFogOfWarManager::GetFogTexture() const
{
    return FogTexture;
}

void AFogOfWarManager::InitFog()
{
    FogTexture = UTexture2D::CreateTransient(TextureSize, TextureSize, PF_B8G8R8A8);
    FogTexture->MipGenSettings = TMGS_NoMipmaps;
    FogTexture->CompressionSettings = TC_VectorDisplacementmap;
    FogTexture->SRGB = false;
    FogTexture->AddToRoot();
    FogTexture->UpdateResource();

    Pixels.SetNumZeroed(TextureSize * TextureSize);
    for (FColor& Color : Pixels)
    {
        Color = FColor::Black;
    }
    UpdateTexture();
}

TArray<uint8> AFogOfWarManager::GetFogAsBitmask() const
{
    TArray<uint8> Bitmask;
    const int32 NumPixels = Pixels.Num();
    const int32 NumBytes = (NumPixels + 7) / 8;
    Bitmask.Init(0, NumBytes);

    for (int32 i = 0; i < NumPixels; ++i)
    {
        if (Pixels[i].R > 0)
        {
            int32 ByteIndex = i / 8;
            int32 BitOffset = i % 8;
            Bitmask[ByteIndex] |= (1 << BitOffset);
        }
    }

    return Bitmask;
}

void AFogOfWarManager::SetFogFromBitmask(const TArray<uint8>& Bitmask)
{
    const int32 NumPixels = TextureSize * TextureSize;
    if (Bitmask.Num() * 8 < NumPixels)
    {
        return;
    }

    Pixels.SetNumZeroed(NumPixels);

    for (int32 i = 0; i < NumPixels; ++i)
    {
        const int32 ByteIndex = i / 8;
        const int32 BitIndex = i % 8;
        const bool bRevealed = (Bitmask[ByteIndex] >> BitIndex) & 1;
        Pixels[i] = bRevealed ? FColor::White : FColor::Black;
    }

    UpdateTexture();
}
