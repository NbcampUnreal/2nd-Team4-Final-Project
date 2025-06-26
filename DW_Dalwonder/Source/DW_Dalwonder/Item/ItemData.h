#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "EItemType.h"
#include "EquipSlotType.h"
#include "EConsumableEffectType.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FConsumableEffectData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Effect")
    EConsumableEffectType EffectType; // 이 효과가 어떤 종류인지

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Effect")
    float EffectValue; // 효과의 수치 (예: 50 체력, 10% 공격력 등)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Effect")
    float Duration; // 버프의 지속 시간 (초)

    FConsumableEffectData()
        : EffectType(EConsumableEffectType::None)
        , EffectValue(0.0f)
        , Duration(0.0f)
    {
    }
};

USTRUCT(BlueprintType)
struct FEquipmentStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Stats")
    float MaxHealthBonus = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Stats")
    float HealthGenRateBonus = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Stats")
    float MaxStaminaBonus = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Stats")
    float StaminaGenRateBonus = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Stats")
    float AttackBonus = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Stats")
    float DefenseBonus = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Stats")
    float MaxWeightBonus = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Stats")
    float AttackSpeedBonus = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Stats")
    float WalkSpeedBonus = 0.f;
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    EItemType ItemType = EItemType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<UStaticMesh> ItemMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    int32 MaxStackSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    float Weight;

    // 소비 아이템 전용: 다양한 효과 목록
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data | Consumable")
    TArray<FConsumableEffectData> ConsumableEffects;

    // 장비 스탯 (EquippableItem에만 관련)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data | Equipment")
    FEquipmentStats EquipmentStats;

    // 장착 아이템 전용: 어떤 슬롯에 장착되는지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data | Equipment")
    EEquipSlotType EquipSlot;
    
    // 무기 전용: 캐릭터 스켈레톤에 부착될 소켓 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data | Equipment")
    FName AttachSocketName;
};
