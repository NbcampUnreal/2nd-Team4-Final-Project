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
	virtual void Interact_Implementation(AActor* Interactor) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void ShowInteractionWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void HideInteractionWidget();


};