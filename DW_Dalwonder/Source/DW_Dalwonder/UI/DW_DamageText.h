#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_DamageText.generated.h"

class UWidgetComponent;

UCLASS()
class DW_DALWONDER_API ADW_DamageText : public AActor
{
	GENERATED_BODY()
	
public:	
	ADW_DamageText();

    void SetDamage(float Damage);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    UWidgetComponent* WidgetComponent;
};