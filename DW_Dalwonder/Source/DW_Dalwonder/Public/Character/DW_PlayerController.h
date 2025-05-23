#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DW_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;

UCLASS()
class DW_DALWONDER_API ADW_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADW_PlayerController();

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* GuardAction;

protected:
	// HUD
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> HUDWidgetClass;

private:
	//HUD
	UPROPERTY()
	UUserWidget* HUDWidgetInstance;
};
