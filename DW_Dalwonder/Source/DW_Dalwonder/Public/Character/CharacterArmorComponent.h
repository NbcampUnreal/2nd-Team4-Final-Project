#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterArmorComponent.generated.h"

class UItemDataManager;

UENUM(BlueprintType)
enum class ECharacterArmor : uint8
{
	Helmet	UMETA(DisplayName = "Helmet"),
	Armor	UMETA(DisplayName = "Armor"),
	Pants	UMETA(DisplayName = "Pants"),
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

	void EquipArmor(UItemBase* Item);

	UStaticMesh* GetItemStaticMesh(UItemBase* Item) const;

	USkeletalMesh* GetItemSkeletalMesh(UItemBase* Item) const;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Helmet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Armor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Pants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Glove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Boots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	UItemBase* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	ADW_CharacterBase* Character;

	UPROPERTY()
	UItemDataManager* ItemDataManager;
};
