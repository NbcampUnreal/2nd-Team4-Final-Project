// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DW_Hp_Test.generated.h"

class UProgressBar;

UCLASS()
class DW_DALWONDER_API UDW_Hp_Test : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // 체력 감소
    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount = 10.0f);

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    // 최대 체력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.0f;

    // 현재 체력
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    // UI 업데이트 함수
    void UpdateHealthBar();
};