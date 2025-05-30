// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EItemType.h"
#include "EItemgrade.h"
#include "ItemTranslator.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UItemTranslator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Item Code")
    static int32 MakeItemCode(EItemGrade ItemGrade, int32 EnchantLevel, int32 ItemRowID);

    UFUNCTION(BlueprintCallable, Category = "Item Code", Meta = (AutoCreateRefTerm = "ItemCode"))
    static void ParseItemCode(int32 ItemCode, EItemGrade& OutItemGrade, int32& OutEnchantLevel, int32& OutItemRowID, bool& OutIsSuccess);
	
};
