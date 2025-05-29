#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BearableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBearableInterface : public UInterface
{
	GENERATED_BODY()
};

class DW_DALWONDER_API IBearableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sword")
	bool CanBeCut(const FHitResult& Hit);
};
