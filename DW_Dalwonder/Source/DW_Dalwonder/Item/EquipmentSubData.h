#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EquipmentSubData.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
    None        UMETA(DisplayName = "None"), // 기본값
    Weapon      UMETA(DisplayName = "Weapon"),
    Helmet      UMETA(DisplayName = "Helmet"),
    UpperBody   UMETA(DisplayName = "UpperBody"),
    LowerBody   UMETA(DisplayName = "LowerBody"),
    Boots       UMETA(DisplayName = "Boots")
};

USTRUCT(BlueprintType)
struct FEquipmentSubData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Data")
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Data")
    float AttackDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Data")
    float Defense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Data")
    EEquipmentSlot EquipmentSlot;
};
