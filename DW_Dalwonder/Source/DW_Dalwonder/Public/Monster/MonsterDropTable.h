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
	FString ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseMinDropCount = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseMinDropCount"))
	int32 MinDropCount = 1;

};

USTRUCT(BlueprintType)
struct FMonsterDropTable : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDropItemData> DropItems;
};