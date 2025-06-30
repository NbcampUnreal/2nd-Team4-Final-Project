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
        StatComponent = Owner->FindComponentByClass<UCharacterStatComponent>();
        if (!StatComponent)
        {
            UE_LOG(LogTemp, Error, TEXT("[Attribute] StatComponent not found on %s"), *Owner->GetName());
            return;
        }

        // StatComponent에서 Base값 가져와서 AttributeComponent에 복사
        BaseMaxStamina = StatComponent->GetBaseMaxStamina();
        BaseMoveSpeed = StatComponent->GetBaseWalkSpeed(); // 걷는 속도를 이동속도로 사용
        BaseMaxCarryWeight = StatComponent->GetBaseMaxWeight();
        BaseMaxHealth = StatComponent->GetBaseMaxHealth();
        BaseHealthRegen = StatComponent->GetBaseHealthGenRate();
        BaseStaminaRegen = StatComponent->GetBaseStaminaGenRate();
        BaseDefense = StatComponent->GetBaseDefense();

		// 추가데미지는 공격력 관련
        /*BaseDamageToLowHPEnemies = StatComponent->GetAttack();
        BaseDamageToHighHPEnemies = StatComponent->GetAttack();
        BaseDamageToNormalEnemies = StatComponent->GetAttack();
        BaseDamageToBoss = StatComponent->GetAttack();*/
    }
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
