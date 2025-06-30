// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_InteractItemBase.h"
#include "DW_SavePoint.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API ADW_SavePoint : public ADW_InteractItemBase
{
	GENERATED_BODY()

public:
    ADW_SavePoint();

protected:
    virtual void Interact_Implementation(AActor* Interactor) override;

    //
    UFUNCTION(BlueprintCallable, Category = "SavePoint")
    void ExecuteSave();

    //Save소리
    UPROPERTY(EditAnywhere, Category = "SavePoint")
    USoundBase* SaveSound;
};
