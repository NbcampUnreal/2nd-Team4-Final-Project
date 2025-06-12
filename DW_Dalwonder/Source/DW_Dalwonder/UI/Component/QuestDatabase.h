// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "UI/Component/Struct/QuestData.h"
#include "QuestDatabase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DW_DALWONDER_API UQuestDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
    /** DataTable 에셋 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    UDataTable* QuestDataTable;

    /** QuestID로 퀘스트 검색 */
    UFUNCTION(BlueprintCallable)
    FQuestData FindQuestByID(FName QuestID) const;

};
