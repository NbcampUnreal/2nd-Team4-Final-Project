#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterArmorComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterArmor : uint8
{
	Helmet	UMETA(DisplayName = "Helmet"),
	Armor	UMETA(DisplayName = "Armor"),
	Glove	UMETA(DisplayName = "Glove"),
	Boots	UMETA(DisplayName = "Boots"),
	Weapon	UMETA(DisplayName = "Weapon")
};

class UItemBase;
class ADW_CharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DW_DALWONDER_API UCharacterArmorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterArmorComponent();

protected:
	virtual void BeginPlay() override;

	bool EquipArmor(UItemBase* Item);

	void AdjustArmorMesh(ECharacterArmor ArmorType);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Helmet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Glove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Boots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	ADW_CharacterBase* Character;
};
