#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "DW_SoundBase.generated.h"

class UAudioComponent;
class USceneComponent;
class UArrowComponent;

UCLASS()
class DW_DALWONDER_API ADW_SoundBase : public AActor
{
	GENERATED_BODY()
	
public:
    ADW_SoundBase();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void UpdateVolume();
    float GetClosestDistanceToMarkingPoints() const;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* SoundAsset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
    UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    float MaxRange = 2000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    float MinVolume = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    float MaxVolume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    float VolumeUpdateInterval = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    TArray<AActor*> MarkingPointActors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    FName MarkingPointTag;

private:
    AActor* PlayerActor;
    FTimerHandle VolumeUpdateTimer;
};