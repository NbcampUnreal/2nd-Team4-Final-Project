#include "DW_AttributeComponent.h"
#include "DW_AttributeSaveData.h"
#include "Character/CharacterStatComponent.h"

UDW_AttributeComponent::UDW_AttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDW_AttributeComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        if (!StatComponent)
        {
            StatComponent = Owner->FindComponentByClass<UCharacterStatComponent>();
        }

        // StatComponent에서 Base값 가져와서 AttributeComponent에 복사
        BaseMaxStamina = StatComponent->GetBaseMaxStamina();
        BaseMoveSpeed = StatComponent->GetBaseWalkSpeed(); // 걷는 속도를 이동속도로 사용
        BaseMaxCarryWeight = StatComponent->GetBaseMaxWeight();
        BaseMaxHealth = StatComponent->GetBaseMaxHealth();
        BaseHealthRegen = StatComponent->GetBaseHealthGenRate();
        BaseStaminaRegen = StatComponent->GetBaseStaminaGenRate();
        BaseDefense = StatComponent->GetBaseDefense();

        // Attribute값 Stat으로 보내기
        StatComponent->SetAttrBonus(EAttrType::MaxHealth, BonusMaxHealth);
        StatComponent->SetAttrBonus(EAttrType::MaxStamina, BonusMaxStamina);
        StatComponent->SetAttrBonus(EAttrType::StaminaRegen, BonusStaminaRegen);
        StatComponent->SetAttrBonus(EAttrType::HealthRegen, BonusHealthRegen);
        StatComponent->SetAttrBonus(EAttrType::Defense, BonusDefense);
    }
}

void UDW_AttributeComponent::ApplyAllBonusToStat()
{
    if (!StatComponent) return;

    StatComponent->SetAttrBonus(EAttrType::MaxHealth, BonusMaxHealth);
    StatComponent->SetAttrBonus(EAttrType::MaxStamina, BonusMaxStamina);
    StatComponent->SetAttrBonus(EAttrType::HealthRegen, BonusHealthRegen);
    StatComponent->SetAttrBonus(EAttrType::StaminaRegen, BonusStaminaRegen);
    StatComponent->SetAttrBonus(EAttrType::Defense, BonusDefense);
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
	BonusLongswordDamageMod = 0.f;
	BonusGreatswordDamageMod = 0.f;
	BonusDamageToLowHPEnemies = 0.f;
	BonusDamageToHighHPEnemies = 0.f;
	BonusDamageToNormalEnemies = 0.f;
	BonusDamageToBoss = 0.f;
	bHasLastStandSkill = false; // 마지막 저항 스킬 초기화
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
	OutData.BaseLongswordDamageMod = BaseLongswordDamageMod;
	OutData.BaseGreatswordDamageMod = BaseGreatswordDamageMod;
	OutData.BaseDamageToLowHPEnemies = BaseDamageToLowHPEnemies;
	OutData.BaseDamageToHighHPEnemies = BaseDamageToHighHPEnemies;
	OutData.BaseDamageToNormalEnemies = BaseDamageToNormalEnemies;
	OutData.BaseDamageToBoss = BaseDamageToBoss;

	// 스킬 관련 데이터 저장
	OutData.bHasLastStandSkill = bHasLastStandSkill;
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
	BaseLongswordDamageMod = InData.BaseLongswordDamageMod;
	BaseGreatswordDamageMod = InData.BaseGreatswordDamageMod;
	BaseDamageToLowHPEnemies = InData.BaseDamageToLowHPEnemies;
	BaseDamageToHighHPEnemies = InData.BaseDamageToHighHPEnemies;
	BaseDamageToNormalEnemies = InData.BaseDamageToNormalEnemies;
	BaseDamageToBoss = InData.BaseDamageToBoss;

	// 스킬 관련 데이터 로드
	bHasLastStandSkill = InData.bHasLastStandSkill;
	// 보너스 초기화 따로 로드를 해주기때문
	ClearAllBonuses();
}
