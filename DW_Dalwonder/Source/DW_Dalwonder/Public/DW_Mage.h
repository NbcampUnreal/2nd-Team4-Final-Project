#pragma once

#include "CoreMinimal.h"
#include "DW_CharacterBase.h"
#include "DW_Mage.generated.h"

class ADW_SpellProjectile;

UCLASS()
class DW_DALWONDER_API ADW_Mage : public ADW_CharacterBase
{
	GENERATED_BODY()

public:
	ADW_Mage();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void SpawnProjectile(TSubclassOf<ADW_SpellProjectile>& Spell);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spell")
	TArray<ADW_SpellProjectile*> UsableSpells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FVector MuzzleOffset;
};
