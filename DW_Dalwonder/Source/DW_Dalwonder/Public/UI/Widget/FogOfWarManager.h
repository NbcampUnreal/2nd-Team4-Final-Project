#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "FogOfWarManager.generated.h"

UCLASS()
class DW_DALWONDER_API AFogOfWarManager : public AActor
{
	GENERATED_BODY()

public:
	AFogOfWarManager();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateFogAtPlayerLocation(const FVector& WorldLocation);
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetFogTexture() const;

	UFUNCTION(BlueprintCallable)
	void InitFog();

	TArray<uint8> GetFogAsBitmask() const;

	void SetFogFromBitmask(const TArray<uint8>& Bitmask);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fog)
	FVector2D GridOrigin;

protected:
	void UpdateTexture();

protected:
	UPROPERTY()
	UTexture2D* FogTexture;

	UPROPERTY()
	UMaterialInstanceDynamic* FogMaterialInstance;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* FogMaterialBase;

	UPROPERTY(EditAnywhere)
	class UImage* FogImage;

	TArray<FColor> Pixels;

	int32 TextureSize;
	float RevealRadius;
	float PixelSize;
};