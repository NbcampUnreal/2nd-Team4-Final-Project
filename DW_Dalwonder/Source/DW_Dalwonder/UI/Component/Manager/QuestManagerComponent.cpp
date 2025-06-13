// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Component/Manager/QuestManagerComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UQuestManagerComponent::UQuestManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UQuestManagerComponent::AcceptQuest(const FQuestData& QuestData)
{
    if (AcceptedQuests.Contains(QuestData.QuestID))
    {
        return;
    }

    AcceptedQuests.Add(QuestData.QuestID, QuestData);
}

void UQuestManagerComponent::UpdateObjectiveProgress(FName TargetID, int32 Amount)
{
    for (auto& Elem : AcceptedQuests)
    {
        FQuestData& Quest = Elem.Value;

        if (Quest.bIsCompleted)
        {
            continue; // 이미 완료된 퀘스트는 스킵
        }

        bool bAnyUpdated = false;

        for (FQuestObjective& Objective : Quest.Objectives)
        {
            if (Objective.TargetID == TargetID && !Objective.IsCompleted())
            {
                Objective.CurrentCount += Amount;
                bAnyUpdated = true;
            }
        }

        // 퀘스트 전체 완료 여부 검사
        if (IsQuestCompleted(Quest))
        {
            CompleteQuest(Elem.Key); // 퀘스트 완료 처리
        }
    }
}

bool UQuestManagerComponent::IsQuestCompleted(const FQuestData& Quest)
{
    for (const FQuestObjective& Obj : Quest.Objectives)
    {
        if (Obj.CurrentCount < Obj.RequiredCount)
        {
            return false;
        }
    }
    return true;
}

void UQuestManagerComponent::CompleteQuest(FName QuestID)
{
    if (FQuestData* Quest = AcceptedQuests.Find(QuestID))
    {
        UE_LOG(LogTemp, Log, TEXT("퀘스트 [%s] 완료!"), *QuestID.ToString());

        // 보상 지급 (추후 확장)
        // ...

        CompletedQuests.Add(QuestID, *Quest);
        AcceptedQuests.Remove(QuestID);
    }
}

