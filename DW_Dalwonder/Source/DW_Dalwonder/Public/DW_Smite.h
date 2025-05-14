#pragma once

#include "CoreMinimal.h"
#include "DW_InteractItemBase.h"
#include "Blueprint/UserWidget.h"
#include "DW_Smite.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Smite : public ADW_InteractItemBase
{
    GENERATED_BODY()

public:
    ADW_Smite();

protected:
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> SmiteWidgetClass;

public:
    virtual void Interact_Implementation(AActor* Interactor) override;
};