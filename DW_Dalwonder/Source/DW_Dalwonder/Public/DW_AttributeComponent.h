#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DW_AttributeComponent.generated.h"



struct FAttributeSaveData;
class UCharacterStatComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DW_DALWONDER_API UDW_AttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDW_AttributeComponent();

    UPROPERTY()
    UCharacterStatComponent* StatComponent;

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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseLongswordDamageMod = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseGreatswordDamageMod = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseDamageToLowHPEnemies = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseDamageToHighHPEnemies = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseDamageToNormalEnemies = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base") float BaseDamageToBoss = 0.f;

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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusLongswordDamageMod = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusGreatswordDamageMod = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusDamageToLowHPEnemies = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusDamageToHighHPEnemies = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusDamageToNormalEnemies = 0.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus") float BonusDamageToBoss = 0.f;

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
    UFUNCTION(BlueprintCallable) float GetLongswordDamageMod() const { return BaseLongswordDamageMod + BonusLongswordDamageMod; }
    UFUNCTION(BlueprintCallable) float GetGreatswordDamageMod() const { return BaseGreatswordDamageMod + BonusGreatswordDamageMod; }
    UFUNCTION(BlueprintCallable) float GetDamageToLowHPEnemies() const { return BaseDamageToLowHPEnemies + BonusDamageToLowHPEnemies; }
    UFUNCTION(BlueprintCallable) float GetDamageToHighHPEnemies() const { return BaseDamageToHighHPEnemies + BonusDamageToHighHPEnemies; }
    UFUNCTION(BlueprintCallable) float GetDamageToNormalEnemies() const { return BaseDamageToNormalEnemies + BonusDamageToNormalEnemies; }
    UFUNCTION(BlueprintCallable) float GetDamageToBoss() const { return BaseDamageToBoss + BonusDamageToBoss; }

	/* ==================== Skills ================== */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	bool bHasLastStandSkill = false;    // 마지막 저항 스킬 보유 여부

    // Bonus 한번에 넘겨주기
    UFUNCTION(BlueprintCallable, Category = "Attribute | Sync")
    void ApplyAllBonusToStat();

    void ClearAllBonuses();

    //Save and Load
    void SaveData(struct FAttributeSaveData& OutData) const;
    void LoadData(const struct FAttributeSaveData& InData);

protected:
    virtual void BeginPlay() override;
};
