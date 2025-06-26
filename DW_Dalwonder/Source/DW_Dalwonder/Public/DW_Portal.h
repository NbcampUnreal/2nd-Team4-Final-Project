// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_InteractItemBase.h"
#include "DW_Portal.generated.h"
UENUM(BlueprintType)
enum class EPortalType : uint8
{
	Portal1 UMETA(DisplayName = "Portal1"),
	Portal2 UMETA(DisplayName = "Portal2"),
	Portal3 UMETA(DisplayName = "Portal3"),
	Portal4 UMETA(DisplayName = "Portal4"),
	Portal5 UMETA(DisplayName = "Portal5"),
	Portal6 UMETA(DisplayName = "Portal6"),
	Portal7 UMETA(DisplayName = "Portal7"),
	Portal8 UMETA(DisplayName = "Portal8"),
	Portal9 UMETA(DisplayName = "Portal9"),
	Portal10 UMETA(DisplayName = "Portal10"),
	None UMETA(Hidden)
};

UCLASS()
class DW_DALWONDER_API ADW_Portal : public ADW_InteractItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADW_Portal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	EPortalType PortalType = EPortalType::Portal1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Portal")
	TSoftObjectPtr<UWorld> TargetLevel;

	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
