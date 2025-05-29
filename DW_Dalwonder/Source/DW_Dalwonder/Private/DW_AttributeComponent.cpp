#include "DW_AttributeComponent.h"

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
