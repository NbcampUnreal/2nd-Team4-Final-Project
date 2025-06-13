// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DW_InteractItemBase.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "DW_Mimic.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_Mimic : public ADW_NormalMonsterBase, public IDW_InteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_Mimic();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsClosed = true;
	
	virtual void Interact_Implementation(AActor* Interactor) override;
};
