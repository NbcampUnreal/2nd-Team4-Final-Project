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

class AWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DW_DALWONDER_API UCharacterArmorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterArmorComponent();

protected:
	virtual void BeginPlay() override;

	bool EquipArmor(AWeaponBase* Item);

	void AdjustArmorMesh(AWeaponBase* Item, ECharacterArmor ArmorType);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	USkeletalMesh* Helmet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	USkeletalMesh* Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	USkeletalMesh* Glove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	USkeletalMesh* Boots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	USceneComponent* Weapon;
};
