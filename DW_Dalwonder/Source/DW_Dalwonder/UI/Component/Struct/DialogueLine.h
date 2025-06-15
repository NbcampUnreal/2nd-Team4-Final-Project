// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueLine.generated.h"

UENUM(BlueprintType)
enum class EQuestDialogueStage : uint8
{
    BeforeAccept,   //  퀘스트 수락전
    InProgress,     //  퀘스트 진행중
    Completed,      //  퀘스트 완료
    None            //  아무것도 아닌 NPC
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDialogueLine : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText SpeakerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DialogueText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsQuestDialogue = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName QuestID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EQuestDialogueStage DialogueStage = EQuestDialogueStage::None;  // 기본값 아무것도 아닌상태
};
