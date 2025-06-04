#include "DW_AttributeComponent.h"
#include "DW_AttributeSaveData.h"

UDW_AttributeComponent::UDW_AttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDW_AttributeComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UDW_AttributeComponent::ClearAllBonuses()
{
    BonusMaxStamina = 0.f;
    BonusMoveSpeed = 0.f;
    BonusMaxCarryWeight = 0.f;
    BonusMaxHealth = 0.f;
    BonusHealthRegen = 0.f;
    BonusStaminaRegen = 0.f;
    BonusLongswordXPMod = 0.f;
    BonusGreatswordXPMod = 0.f;
    BonusDefense = 0.f;
}

void UDW_AttributeComponent::SaveData(FAttributeSaveData& OutData) const
{
    OutData.BaseMaxStamina = BaseMaxStamina;
    OutData.BaseMoveSpeed = BaseMoveSpeed;
    OutData.BaseMaxCarryWeight = BaseMaxCarryWeight;
    OutData.BaseMaxHealth = BaseMaxHealth;
    OutData.BaseHealthRegen = BaseHealthRegen;
    OutData.BaseStaminaRegen = BaseStaminaRegen;
    OutData.BaseLongswordXPMod = BaseLongswordXPMod;
    OutData.BaseGreatswordXPMod = BaseGreatswordXPMod;
    OutData.BaseDefense = BaseDefense;
}

void UDW_AttributeComponent::LoadData(const FAttributeSaveData& InData)
{
    BaseMaxStamina = InData.BaseMaxStamina;
    BaseMoveSpeed = InData.BaseMoveSpeed;
    BaseMaxCarryWeight = InData.BaseMaxCarryWeight;
    BaseMaxHealth = InData.BaseMaxHealth;
    BaseHealthRegen = InData.BaseHealthRegen;
    BaseStaminaRegen = InData.BaseStaminaRegen;
    BaseLongswordXPMod = InData.BaseLongswordXPMod;
    BaseGreatswordXPMod = InData.BaseGreatswordXPMod;
    BaseDefense = InData.BaseDefense;
}
