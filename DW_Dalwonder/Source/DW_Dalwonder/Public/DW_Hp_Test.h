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

    // ü�� ����
    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount = 10.0f);

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    // �ִ� ü��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.0f;

    // ���� ü��
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    // UI ������Ʈ �Լ�
    void UpdateHealthBar();
};