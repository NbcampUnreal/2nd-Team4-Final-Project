#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "DW_SwordBase.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_SwordBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	ADW_SwordBase();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	USceneComponent* SwordTraceStartPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	USceneComponent* SwordTraceEndPoint;
};
