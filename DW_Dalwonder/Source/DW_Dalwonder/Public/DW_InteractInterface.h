#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DW_InteractInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDW_InteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DW_DALWONDER_API IDW_InteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Interactor);
};