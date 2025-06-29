// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerInfoWidget.h"
#include "UI/Widget/CustomButtonWidget.h"
#include "DW_GmBase.h"
#include "Character/DW_CharacterBase.h"
#include "DW_AttributeComponent.h"
#include "Character/CharacterStatComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Character/CharacterArmorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SceneCapture2D.h"

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 캐릭터 참조
    if (ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        float FinalDamage = 0;

        if (const UDW_AttributeComponent* Attr = Player->AttributeComponent)
        {
            if (const UCharacterStatComponent* Stat = Player->GetCharacterStatComponent())
            {
                const float MaxHP = Stat->GetTotalMaxHealth() + Attr->BonusMaxHealth;
                const float MaxStamina = Stat->GetTotalMaxStamina() + Attr->BonusMaxStamina;
                const float Defense = Stat->GetTotalDefense() + Attr->BonusDefense;
                const float Weight = Stat->GetTotalMaxWeight() + Attr->BonusMaxCarryWeight;
                const float MoveSpeed = Stat->GetTotalWalkSpeed() + Attr->BonusMoveSpeed;
                const float HPRegen = Stat->GetTotalHealthGenRate() + Attr->BonusHealthRegen;
                const float STRegen = Stat->GetTotalStaminaGenRate() + Attr->BonusStaminaRegen;

                // 기본 공격력
                float Attack = Stat->GetTotalAttack();

                // 무기 코드 기반 장/대검 보너스 데미지 추가
                if (const auto* Armor = Player->FindComponentByClass<UCharacterArmorComponent>())
                {
                    if (Armor->Weapon)
                    {
                        FString ItemCodeStr = Armor->Weapon->ItemCode;
                        if (ItemCodeStr.Len() > 3)
                        {
                            FString WeaponCodeStr = ItemCodeStr.Mid(3);
                            int32 WeaponCode = FCString::Atoi(*WeaponCodeStr);

                            if ((WeaponCode >= 1 && WeaponCode <= 11) || WeaponCode == 23 || WeaponCode == 24)
                            {
                                Attack += Attr->BonusGreatswordDamageMod;
                            }
                            else if ((WeaponCode >= 12 && WeaponCode <= 22) || WeaponCode == 25 || WeaponCode == 26)
                            {
                                Attack += Attr->BonusLongswordDamageMod;
                            }
                        }
                    }
                }

                UpdatePlayerInfo(MaxHP, MaxStamina, Attack, Defense, Weight, MoveSpeed, HPRegen, STRegen);
            }
        }
    }

}

void UPlayerInfoWidget::UpdatePlayerInfo(float MaxHP, float MaxStamina, float Damage, float Defense, float Weight, float MoveSpeed, float HPRegen, float StaminaRegen)
{
	if (HPText)        HPText->SetText(FText::FromString(FString::Printf(TEXT("HP : %.0f"), MaxHP)));
	if (SteminaText)   SteminaText->SetText(FText::FromString(FString::Printf(TEXT("Stamina : %.0f"), MaxStamina)));
	if (DamageText)    DamageText->SetText(FText::FromString(FString::Printf(TEXT("Damage : %.0f"), Damage)));
	if (DefenceText)   DefenceText->SetText(FText::FromString(FString::Printf(TEXT("Defense : %.0f"), Defense)));
	if (WeightText)    WeightText->SetText(FText::FromString(FString::Printf(TEXT("Weight : %.0f"), Weight)));
	if (MoveSpeedText) MoveSpeedText->SetText(FText::FromString(FString::Printf(TEXT("Move Speed : %.0f"), MoveSpeed)));
	if (HPRegenText)   HPRegenText->SetText(FText::FromString(FString::Printf(TEXT("HP Regen : %.1f"), HPRegen)));
	if (StRegenText)   StRegenText->SetText(FText::FromString(FString::Printf(TEXT("Stamina Regen : %.1f"), StaminaRegen)));
}

