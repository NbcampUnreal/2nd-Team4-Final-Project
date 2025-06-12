// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item/WorldItemActor.h"
#include "MonsterDropTable.generated.h"

USTRUCT(BlueprintType)
struct FDropItemData
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWorldItemActor> DropItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropChance;

};

USTRUCT(BlueprintType)
struct FMonsterDropTable : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDropItemData> DropItems;
};