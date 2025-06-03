#pragma once

#include "CoreMinimal.h"
#include "DW_CharacterBase.h"
#include "DW_Warrior.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Warrior : public ADW_CharacterBase
{
	GENERATED_BODY()

public:
	ADW_Warrior();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
};
