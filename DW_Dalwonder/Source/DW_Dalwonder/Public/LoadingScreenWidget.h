#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenWidget.generated.h"


UCLASS()
class DW_DALWONDER_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION(BlueprintCallable)
    void SetLoadingPercent(float Percent);

protected:
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ProgressBar_Loading;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Tip;


private:
    FTimerHandle TipChangeTimer;

    TArray<FString> TipList;
    int32 CurrentTipIndex = 0;

    void ChangeTipText();
};