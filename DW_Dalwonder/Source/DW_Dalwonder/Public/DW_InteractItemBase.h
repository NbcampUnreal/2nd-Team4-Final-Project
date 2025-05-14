// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_InteractInterface.h"
#include "DW_InteractItemBase.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_InteractItemBase : public AActor , public IDW_InteractInterface
{
	GENERATED_BODY()
	
public:	
	ADW_InteractItemBase();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

public:	
	virtual void Interact_Implementation(AActor* Interactor) override;

};
