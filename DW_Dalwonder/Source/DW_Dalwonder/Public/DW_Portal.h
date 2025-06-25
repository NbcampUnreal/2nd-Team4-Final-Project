#pragma once

#include "CoreMinimal.h"
#include "DW_InteractItemBase.h"
#include "GameFramework/Actor.h"
#include "DW_Portal.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Portal : public ADW_InteractItemBase
{
	GENERATED_BODY()
	
public:	
	ADW_Portal();

protected:


public:
	virtual void Interact_Implementation(AActor* Interactor) override;

};
