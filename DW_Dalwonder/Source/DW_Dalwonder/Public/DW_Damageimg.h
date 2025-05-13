#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DW_Damageimg.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDW_Damageimg : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
	void UpdateDamageani(float DamageAmount);
};
