#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraSystem.h"
#include "Starcatcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStarCatcherFinished, int32, SuccessCount);

UCLASS()
class DW_DALWONDER_API UStarcatcher : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    UFUNCTION(BlueprintCallable)
    void HandleInput();

    UFUNCTION(BlueprintCallable)
    void RestartGame();

    UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Starcatcher")
    class UCanvasPanel* StarCanvas;

    UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Starcatcher")
    class UImage* StarImage;

    UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Starcatcher")
    class UImage* SuccessZoneImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher")
    float StarMoveSpeed = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starcatcher")
    float SuccessZoneWidth = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TArray<UNiagaraSystem*> SuccessEffects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UNiagaraSystem* FailEffect = nullptr;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStarCatcherFinished OnStarCatcherFinished;

    UPROPERTY(BlueprintReadOnly)
    int32 FinalSuccessCount;

    UFUNCTION()
    void EndRound(bool bDelayRestart);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* SuccessSound = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* FailSound = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* MakingSound = nullptr;

protected:
    void InitSuccessZone();
    void FinishGame();

    float CurrentStarX = 0.f;
    bool bGoingRight = true;

    int32 SuccessCount = 0;
    int32 CurrentTry = 0;    
    int32 CurrentGame = 0;   
    int32 MaxTry = 1;        
    int32 MaxGame = 3;       
    FTimerHandle RestartTimerHandle;

    float SuccessLeft = 0.f;
    float SuccessRight = 0.f;
    bool bZoneReady = false;
    bool bIsGameFinished = false;
};
