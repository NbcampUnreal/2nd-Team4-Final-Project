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

	bool EquipArmor(UItemBase* Item);

protected:
	virtual void BeginPlay() override;

	UStaticMesh* GetItemStaticMesh(UItemBase* Item) const;

	USkeletalMesh* GetItemSkeletalMesh(UItemBase* Item) const;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	ADW_CharacterBase* Character;

	UPROPERTY()
	UItemDataManager* ItemDataManager;
};
