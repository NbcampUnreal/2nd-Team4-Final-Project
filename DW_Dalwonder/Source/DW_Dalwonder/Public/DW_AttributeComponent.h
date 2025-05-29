#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DW_AttributeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DW_DALWONDER_API UDW_AttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDW_AttributeComponent();

    /* ==================== Base ==================== */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseMaxStamina = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseMoveSpeed = 600.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseMaxCarryWeight = 50.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseMaxHealth = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseHealthRegen = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseStaminaRegen = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseLongswordXPMod = 1.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseGreatswordXPMod = 1.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseDefense = 0.f;

    /* ==================== Bonus =================== */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusMaxStamina = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusMoveSpeed = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusMaxCarryWeight = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusMaxHealth = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusHealthRegen = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusStaminaRegen = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusLongswordXPMod = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusGreatswordXPMod = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusDefense = 0.f;

    /* ==================== Getter ================== */
    UFUNCTION(BlueprintCallable) float GetMaxStamina()      const { return BaseMaxStamina + BonusMaxStamina; }
    UFUNCTION(BlueprintCallable) float GetMoveSpeed()       const { return BaseMoveSpeed + BonusMoveSpeed; }
    UFUNCTION(BlueprintCallable) float GetMaxCarryWeight()  const { return BaseMaxCarryWeight + BonusMaxCarryWeight; }
    UFUNCTION(BlueprintCallable) float GetMaxHealth()       const { return BaseMaxHealth + BonusMaxHealth; }
    UFUNCTION(BlueprintCallable) float GetHealthRegen()     const { return BaseHealthRegen + BonusHealthRegen; }
    UFUNCTION(BlueprintCallable) float GetStaminaRegen()    const { return BaseStaminaRegen + BonusStaminaRegen; }
    UFUNCTION(BlueprintCallable) float GetLongswordXPMod()  const { return BaseLongswordXPMod + BonusLongswordXPMod; }
    UFUNCTION(BlueprintCallable) float GetGreatswordXPMod() const { return BaseGreatswordXPMod + BonusGreatswordXPMod; }
    UFUNCTION(BlueprintCallable) float GetDefense()         const { return BaseDefense + BonusDefense; }

    void ClearAllBonuses();

protected:
    virtual void BeginPlay() override;
};
