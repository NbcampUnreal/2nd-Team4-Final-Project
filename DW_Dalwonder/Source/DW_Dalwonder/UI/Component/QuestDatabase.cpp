// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Component/QuestDatabase.h"

FQuestData UQuestDatabase::FindQuestByID(FName QuestID) const
{
    if (!QuestDataTable) return FQuestData();

    const FQuestData* FoundRow = QuestDataTable->FindRow<FQuestData>(QuestID, TEXT("Lookup Quest"));
    if (FoundRow)
    {
        return *FoundRow;
    }

    return FQuestData();
}
