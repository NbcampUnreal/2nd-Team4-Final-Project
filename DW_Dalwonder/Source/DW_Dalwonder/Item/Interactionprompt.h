#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactionprompt.generated.h"


UCLASS()
class DW_DALWONDER_API UInteractionprompt : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "World Item Data")
	class AWorldItemActor* OwningWorldItemActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Item Data")
	int32 Count = 0;

	UFUNCTION(BlueprintImplementableEvent, Category = "MyEvents")
	void InitializeData();

	UFUNCTION(BlueprintImplementableEvent, Category = "MyEvents")
	void SoundPlay();
};
