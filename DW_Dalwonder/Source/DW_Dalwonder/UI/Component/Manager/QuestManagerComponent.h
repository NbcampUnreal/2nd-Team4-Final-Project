// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/Component/Struct/QuestData.h"
#include "QuestManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DW_DALWONDER_API UQuestManagerComponent : public UActorComponent
{
	GENERATED_BODY()


public:
    UQuestManagerComponent();

    /** 퀘스트 수락 */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void AcceptQuest(const FQuestData& Quest);

    /** 퀘스트 진행도 업데이트 */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void UpdateObjectiveProgress(FName TargetID, int32 Amount = 1);

    /** 퀘스트 수락 확인 */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    bool IsQuestAccepted(const FQuestData& Quest) const;

    /** 퀘스트 완료 확인 */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    bool IsQuestCompleted(const FQuestData& Quest);

    /** 퀘스트 완료 처리 */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void CompleteQuest(FName QuestID);

    const TMap<FName, FQuestData>& GetActiveQuests() { return AcceptedQuests; }
    const TMap<FName, FQuestData>& GetCompletedQuests() { return CompletedQuests; }

    /** 현재 수락된 퀘스트 목록 */
    UPROPERTY()
    TMap<FName, FQuestData> AcceptedQuests;

    /** 완료한 퀘스트 목록 */
    UPROPERTY()
    TMap<FName, FQuestData> CompletedQuests;
};
