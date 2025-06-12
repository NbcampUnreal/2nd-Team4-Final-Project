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
        UE_LOG(LogTemp, Warning, TEXT("퀘스트 '%s'는 이미 수락되었습니다."), *QuestData.QuestID.ToString());
        return;
    }

    AcceptedQuests.Add(QuestData.QuestID, QuestData);

    UE_LOG(LogTemp, Log, TEXT("퀘스트 수락됨: %s - %s"),
        *QuestData.QuestID.ToString(),
        *QuestData.Title.ToString());
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

                UE_LOG(LogTemp, Log, TEXT(" 목표 갱신: %s (%d/%d)"),
                    *TargetID.ToString(),
                    Objective.CurrentCount,
                    Objective.RequiredCount);

                bAnyUpdated = true;
            }
        }

        // 모든 목표 완료 여부 확인
        bool bAllCompleted = true;
        for (const FQuestObjective& Obj : Quest.Objectives)
        {
            if (!Obj.IsCompleted())
            {
                bAllCompleted = false;
                break;
            }
        }

        if (bAnyUpdated && bAllCompleted)
        {
            Quest.bIsCompleted = true;
            UE_LOG(LogTemp, Log, TEXT(" 퀘스트 완료됨: %s"), *Quest.Title.ToString());
        }
    }
}

