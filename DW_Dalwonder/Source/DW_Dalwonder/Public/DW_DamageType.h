#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DW_DamageType.generated.h"

UCLASS()
class DW_DALWONDER_API UDW_DamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UDW_DamageType();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageType")
	bool bCanKnockdown = false;
};
